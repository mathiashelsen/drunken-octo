#include "finance.hpp"

void readDatafile(ifstream *file, vector<vector<double> *> *list)
{
    char_separator<char> sep("\t ");
    if(file->is_open())
    {
	string line;
	while( getline( *file, line ) )
	{
	    tokenizer<char_separator<char> > tokens(line, sep);
	    vector<double> *row = new vector<double>;
	    for( tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it )
	    {
		row->push_back(atof(it->c_str()));
	    }
	    list->push_back(row);
	}
    }
}

void createDataset( vector<vector<double> *> *list, 
		    datapoint **root, 
		    vector<datapoint*> **verification )
{
    vector<datapoint *> *myList = new vector<datapoint *>;

    for( vector<vector<double> *>::iterator it = list->begin(); it != list->end(); ++it )
    {
	X x;
	Y y;
	int i = 0;
	for( vector<double>::iterator jt = (*it)->begin(); jt != (*it)->end(); ++jt )
	{
	    if( i < 4 )
	    {
		x.x[i] = *jt;
	    } else {
		y.y[i%4] = *jt;
	    }
	    i++;
	}
	datapoint *ptr = new datapoint(&y, &x);
	assert( ptr != NULL );
	myList->push_back(ptr);
    } 

    //random_shuffle( myList->begin(), myList->end() );

    *verification = new vector<datapoint *>(myList->begin(), myList->begin() + myList->size()/2);
    vector<datapoint *> *training = new vector<datapoint *>(myList->begin() + myList->size()/2, myList->end());

    threadCtrl mgr;
    mgr.curThreads = 1;
    mgr.maxThreads = 1;
    pthread_mutex_init( &(mgr.lock), NULL );
    
    buildTree<Y, X>( training, root, compare, NDIMS, 0, &mgr );

    pthread_mutex_destroy( &(mgr.lock) );
}

void runTest(datapoint *root, 
	     vector<datapoint *> *verification)
{
    int N = verification->size();
    double *weeklyPrediction = new double[N];
    double *weeklyActual = new double[N];
    double *weeklyError = new double[N];

    double *dailyPrediction = new double[N];
    double *dailyActual = new double[N];
    double *dailyError = new double[N];

    for(int i = 0; i < N; i++)
    {
	Sorted_LL<datapoint> *nn = new Sorted_LL<datapoint>(10);
	datapoint *target = verification->at(i);
	Y *targetVal = target->getData();
	X *targetPos = target->getPosition();

	root->findNN( nn, targetPos, metric, projectedDistance, NDIMS, 10 );

	datapoint *ptr = NULL;
	double dist, dailyEstimate, weeklyEstimate, norm;
	dailyEstimate = 0.0;
	weeklyEstimate = 0.0;
	norm = 0.0;

	nn->getNext( &ptr, &dist );
	while( ptr != NULL )
	{
	    Y *y = ptr->getData();
	    norm += 1.0/dist; 
	    dailyEstimate += y->y[0]/dist;
	    weeklyEstimate += y->y[1]/dist;
	    nn->getNext( &ptr, &dist );
	}
	dailyEstimate /= norm;
	weeklyEstimate /= norm;
	weeklyPrediction[i] = weeklyEstimate;
	dailyPrediction[i] = dailyEstimate;
	weeklyActual[i] = targetVal->y[1];
	dailyActual[i] = targetVal->y[0];
	weeklyError[i] = weeklyEstimate - weeklyActual[i];
	dailyError[i] = dailyEstimate - dailyActual[i];

	delete nn;
    }

    double averageReturn = gsl_stats_mean(weeklyActual, 1, N);
    double c00, c01, c10, c11, rsqrd;
    double m,c;
    gsl_fit_linear(weeklyPrediction, 1, weeklyActual, 1, N, &m, &c, &c00, &c01, &c11, &rsqrd);
    std::cout << "Weekly NN prediction" << std::endl;
    std::cout << "m = " << m << " +/- " << sqrt(c00*rsqrd) << " (" << 100.0*sqrt(c00*rsqrd)/m << "\%)" << std::endl;
    std::cout << "c = " << c << " +/- " << sqrt(c11*rsqrd) << " (" << 100.0*sqrt(c11*rsqrd)/c << "\%)" << std::endl;
    std::cout << "Average error : " << gsl_stats_mean(weeklyError, 1, N) << ", ";
    std::cout << gsl_stats_sd(weeklyError, 1, N) << ", ";
    std::cout << gsl_stats_sd(weeklyError, 1, N)/gsl_stats_mean(weeklyError, 1, N) << std::endl;
    double rho = gsl_stats_correlation(weeklyPrediction, 1, weeklyActual, 1, N);
    double rho_err = (1.0 - rho*rho)/sqrt((double)N);
    std::cout << "Correlation coefficient: " << rho << " +/- " << rho_err << std::endl;


    gsl_fit_linear(dailyPrediction, 1, dailyActual, 1, N, &m, &c, &c00, &c01, &c11, &rsqrd);
    std::cout << "\n\nDaily NN prediction" << std::endl;
    std::cout << "m = " << m << " +/- " << sqrt(c00*rsqrd) << " (" << 100.0*sqrt(c00*rsqrd)/m << "\%)" << std::endl;
    std::cout << "c = " << c << " +/- " << sqrt(c11*rsqrd) << " (" << 100.0*sqrt(c11*rsqrd)/c << "\%)" << std::endl;
    std::cout << "Average error : " << gsl_stats_mean(dailyError, 1, N) << ", ";
    std::cout << gsl_stats_sd(dailyError, 1, N) << ", ";
    std::cout << gsl_stats_sd(dailyError, 1, N)/gsl_stats_mean(dailyError, 1, N) << std::endl;
    rho = gsl_stats_correlation(dailyPrediction, 1, dailyActual, 1, N);
    rho_err = (1.0 - rho*rho)/sqrt((double)N);
    std::cout << "Correlation coefficient: " << rho << " +/- " << rho_err << std::endl;

    delete[] weeklyPrediction;
    delete[] weeklyActual;
    delete[] weeklyError;
    delete[] dailyPrediction;
    delete[] dailyActual;
    delete[] dailyError;
}

int compare( X *a, X *b, int rank)
{
    double *A = (double *)a;
    double *B = (double *)b;
    if( A[rank] < B[rank] )
    {
	return -1;
    }
    if( A[rank] == B[rank] )
    {
	return 0;
    }
    else
    {
	return 1;
    }
}

double metric( X *a, X *b )
{
    double *aval = (double *)a;
    double *bval = (double *)b;
    double retval = 0.0;
    for(int i = 0 ; i < NDIMS; i++ )
    {
	retval += (aval[i]-bval[i]) * (aval[i]-bval[i]);
    }
    return retval;
}

double projectedDistance(X *a, X *b, int rank)
{
    double *aval = (double *)a;
    double *bval = (double *)b;
    return copysign((aval[rank]-bval[rank])*(aval[rank]-bval[rank]), (aval[rank]-bval[rank]) );
}

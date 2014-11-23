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
		    vector<datapoint*> **verification,
		    double *scale )
{
    vector<datapoint *> *myList = new vector<datapoint *>;

    for( vector<vector<double> *>::iterator it = list->begin(); it != list->end(); ++it )
    {
	X x;
	Y y;
	int i = 0;
	for( vector<double>::iterator jt = (*it)->begin(); jt != (*it)->end(); ++jt )
	{
	    if( i < NDIMS )
	    {
		x.x[i] = *jt/scale[i];
	    } else {
		y.y[i%NDIMS] = *jt;
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

double runTest(datapoint *root, 
	     vector<datapoint *> *verification)
{
    int N = verification->size();
    double theilU_avg = 0.0;
    double *dailyActual = new double[N];
    double *prdError = new double[N];
    double *avgError = new double[N];

    for(int i = 0; i < N; i++)
    {
	Sorted_LL<datapoint> *nn = new Sorted_LL<datapoint>(10);
	datapoint *target = verification->at(i);
	Y *targetVal = target->getData();
	X *targetPos = target->getPosition();

	root->findNN( nn, targetPos, metric, projectedDistance, NDIMS, 10 );

	datapoint *ptr = NULL;
	double dist, dailyEstimate, norm;
	dailyEstimate = 0.0;
	norm = 0.0;

	nn->getNext( &ptr, &dist );
	while( ptr != NULL )
	{
	    Y *y = ptr->getData();
	    norm += 1.0/dist; 
	    dailyEstimate += y->y[0]/dist;
	    nn->getNext( &ptr, &dist );
	}
	dailyEstimate /= norm;
	prdError[i] = (dailyEstimate - targetVal->y[0])*(dailyEstimate - targetVal->y[0]);
	dailyActual[i] = targetVal->y[0];

	delete nn;
    }

    double averageReturn = gsl_stats_mean(dailyActual, 1, N);
    for(int i = 0; i < N; i++ )
    {
	datapoint *target = verification->at(i);
	Y *targetVal = target->getData();
	avgError[i] = (averageReturn - targetVal->y[0])*(averageReturn - targetVal->y[0]);
    }

    theilU_avg = sqrt(gsl_stats_mean(prdError, 1, N)/gsl_stats_mean(avgError, 1, N));

    delete[] dailyActual;
    delete[] prdError;
    delete[] avgError;

    return theilU_avg;
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

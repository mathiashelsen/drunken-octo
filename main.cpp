/*
The MIT License (MIT)

Copyright (c) 2014 Mathias Helsen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "drunken_octo.hpp"
#include "sorted_ll.hpp"
#include "timer.hpp"

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <gsl/gsl_rng.h>
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <math.h>

#define NDIMS 4

void benchmark_quickselect();
void benchmark_buildtree();

typedef struct
{
    double x[NDIMS];
} vector;

typedef drunken_octo<double, vector> Node;

int comparef( vector *a, vector *b, int rank);
double metric( vector *a, vector *b );
double pDist(vector *a, vector *b, int rank);
double function(vector *a);

int
main(int argc, char **argv)
{
    double p[NDIMS];
    vector *f = (vector *)p;

    int trainingpoints = 1000;
    int testpoints = 200000;
    std::vector< drunken_octo<double, vector> *> training_data;

    boost::mt19937 *rng = new boost::mt19937();
    static boost::uniform_01<boost::mt19937> generator(*rng);

    for(int i = 0; i < trainingpoints; i++ )
    {
	for(int j = 0; j < NDIMS; j++ )
	{
	    p[j] = generator()*20.0 - 10.0;
	}
	double val = function( f );
	Node *newNode = new Node( &val, f );
	training_data.push_back(newNode);
    }

    Node *root = NULL;
    buildTree<double, vector>( &training_data, &root, &comparef, NDIMS, 0);
    int nn_test = 1;
    
    while( nn_test < 31 )
    {
	int err = 0;
	double avgDist = 0.0;

	Timer tm;
	std::ostringstream ooo;
	std::string str;

	tm.start();

	for(int i = 0; i < testpoints; i++ )
	{
	    Sorted_LL< drunken_octo< double, vector> > *nn = new Sorted_LL< drunken_octo< double, vector> >(nn_test);

	    for(int j = 0; j < NDIMS; j++ )
	    {
		p[j] = generator()*20.0 - 10.0;
	    }
	    double val = function( f );

	    root->findNN( nn, f, metric, pDist, NDIMS, nn_test );
	    drunken_octo<double, vector> *data;
	    double dist;
	    double sumDist = 0.0;
	    double sumVal = 0.0;
	    double *nnVal;
	    nn->getNext( &data, &dist );
	    while( data != NULL )
	    {	
		nnVal = data->getData();
		sumVal += (*nnVal)/(dist);
		//sumVal += *nnVal;
		sumDist += 1.0/(dist);
		avgDist += dist;
		nn->getNext( &data, &dist );
	    }
	    sumVal /= sumDist;
	    //sumVal /= (double) nn_test;
	    sumVal = (sumVal > 0.0) ? 1.0 : -1.0;
	    if( sumVal != val ) err++;
	    delete nn;
	}
	tm.stop();
	std::cout << nn_test << "\t" << 1.0 -( (double)err/(double)testpoints ) << "\t" << tm.duration() << std::endl;
	nn_test++;
    }
    
    delete root;
    return 0;
}


int comparef( vector *a, vector *b, int rank)
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

double metric( vector *a, vector *b )
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

double pDist(vector *a, vector *b, int rank)
{
    double *aval = (double *)a;
    double *bval = (double *)b;
    return copysign((aval[rank]-bval[rank])*(aval[rank]-bval[rank]), (aval[rank]-bval[rank]) );
}


double function(vector *a)
{
    double *A = (double *) a;
    double retval = 1.0;

    if( A[0] > 0.0 )
    {
	retval *= -1.0;
    }
    if( A[1] > 0.0 )
    {
	retval *= -1.0;
    }
    if( A[2] > 0.0 )
    {
	retval *= -1.0;
    }
    if( A[3] > 0.0 )
    {
	retval *= -1.0;
    }
    return retval;
}

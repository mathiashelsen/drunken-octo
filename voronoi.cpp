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

#include "voronoi.hpp"

#define NDIMS 2

typedef struct
{
    double x[NDIMS];
} vector;

typedef drunken_octo<double, vector> Node;

int comparef( vector *a, vector *b, int rank);
double metric( vector *a, vector *b );
double pDist(vector *a, vector *b, int rank);

int vorotest()
{
    double p[NDIMS];
    vector *f = (vector *)p;

    int trainingpoints = 1000;
    std::vector< drunken_octo<double, vector> *> training_data;

    boost::mt19937 *rng = new boost::mt19937();
    static boost::uniform_01<boost::mt19937> generator(*rng);

    for(int i = 0; i < trainingpoints; i++ )
    {
	for(int j = 0; j < NDIMS; j++ )
	{
	    p[j] = generator()*20.0 - 10.0;
	}
	double val = (double) i;
	Node *newNode = new Node( &val, f );
	training_data.push_back(newNode);
	std::cout << p[0] << "\t" << p[1] << std::endl;
    }
    std::cout << std::endl << std::endl;

    Node *root = NULL;
    buildTree<double, vector>( &training_data, &root, &comparef, NDIMS, 0);

   
    for(int i = 0; i < 1000; i++ )
    {
	p[0] = (double)i/50.0 - 10.0;
	for(int j = 0; j < 1000; j++)
	{
	    p[1] = (double)j/50.0 - 10.0;
	    drunken_octo<double, vector> *nn = NULL;
	    double dist = 1.0e6;
	    root->findNN(&nn, &dist, f, metric, pDist, NDIMS);
	    double *data = NULL;
	    data = nn->getData();
	    std::cout << p[0] << "\t" << p[1] << "\t" << *data << std::endl;
	}
    }

    delete root;
    delete rng;

    return EXIT_SUCCESS;
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

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
#include "timer.hpp"

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <gsl/gsl_rng.h>
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <math.h>

void benchmark_quickselect();
void benchmark_buildtree();

typedef struct
{
    double x;
    double y;
} vector;

typedef drunken_octo<vector, vector> Node;

int comparef( vector *a, vector *b, int rank);
double metric( vector *a, vector *b );
double pDist(vector *a, vector *b, int rank);

int
main(int argc, char **argv)
{
    std::vector< drunken_octo<vector, vector> *> nodes;

    double xs[4] = {2.0, 3.0, 6.0, 8.0};
    double ys[4] = {5.0, 8.0, 3.0, 9.0};
    vector f;
    for(int i = 0; i < 4; i++ )
    {
	f.x = xs[i];
	f.y = ys[i];
	Node *newNode = new Node( &f, &f );
	nodes.push_back(newNode);
    }

    Node *root = NULL;
    Node *NN = NULL;
    double distance = 1.0e6;
    
    buildTree<vector, vector>( &nodes, &root, &comparef, 1, 0);
    vector *NNposition = root->getPosition();
    std::cout << "root position: " << NNposition->x << ", " << NNposition->y << std::endl;
    f.x = 3.0;
    f.y = 5.0;
    root->findNN( &NN, &distance, &f, &metric, &pDist, 1 );
    NNposition = NN->getPosition();
    std::cout << "NN position: " << NNposition->x << ", " << NNposition->y << std::endl;
    delete root;

    //benchmark_buildtree();

    //benchmark_quickselect();

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
    return ( (aval[0]-bval[0])*(aval[0]-bval[0])
	+ (aval[1]-bval[1])*(aval[1]-bval[1]) );
}

double pDist(vector *a, vector *b, int rank)
{
    double *aval = (double *)a;
    double *bval = (double *)b;
    return copysign((aval[rank]-bval[rank])*(aval[rank]-bval[rank]), (aval[rank]-bval[rank]) );
}

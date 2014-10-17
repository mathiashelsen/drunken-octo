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

typedef drunken_octo<int, vector> Node;

int comparef( vector *a, vector *b, int rank);
double metric( vector *a, vector *b );
double pDist(vector *a, vector *b, int rank);

int
main(int argc, char **argv)
{
    std::vector< drunken_octo<int, vector> *> nodes;

    //double xs[4] = {2.0, 3.0, 6.0, 8.0};
    //double ys[4] = {5.0, 8.0, 3.0, 9.0};
    vector f;

    boost::mt19937 *rng = new boost::mt19937();
    static boost::uniform_01<boost::mt19937> generator(*rng);


    for(int i = 0; i < 100; i++ )
    {
	f.x = generator()*10.0;
	f.y = generator()*10.0;
	std::cout << f.x << "\t" << f.y << "\t" << i << std::endl;
	Node *newNode = new Node( &i, &f );
	nodes.push_back(newNode);
    }
    std::cout << std::endl << std::endl;

    Node *root = NULL;
    Node *NN = NULL;
    
    buildTree<int, vector>( &nodes, &root, &comparef, 2, 0);

    
    f.x = 9.22;
    f.y = 8.33;

    
    double distance = 1.0e6;
    root->findNN( &NN, &distance, &f, &metric, &pDist, 2 );
    int *t = NN->getData();
    std::cout << "# Found near node " << *t << std::endl;
 
  
   
    for(int i = 0; i < 1000; i++)
    {
	for(int j = 0; j < 1000; j++)
	{
	    f.x = 0.01*(double)i;
	    f.y = 0.01*(double)j;

	    double distance = 1.0e6;
	    root->findNN( &NN, &distance, &f, &metric, &pDist, 2 );
	    int *t = NN->getData();
	    std::cout << f.x << "\t" << f.y << "\t" << *t << std::endl;
	}
    }
    
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
    //std::cout << "Asked to calculate distance between (" << aval[0] << ", " << aval[1] << ") and ";
    //std::cout << "(" << bval[0] << ", " << bval[1] << ")" << std::endl;
    return copysign((aval[rank]-bval[rank])*(aval[rank]-bval[rank]), (aval[rank]-bval[rank]) );
}

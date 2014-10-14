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

int comparef( double *a, double *b, int rank)
{
    if( *a < *b )
    {
	return -1;
    }
    if( *a == *b)
    {
	return 0;
    }
    else
    {
	return 1;
    }
}

typedef drunken_octo<double, double> Node1D;
void benchmark_quickselect();

int
main(int argc, char **argv)
{
    benchmark_quickselect();

    return 0;
}

void benchmark_quickselect()
{
    int sizeList = 1000;
    while(sizeList <= 10000000)
    {
	boost::mt19937 *rng = new boost::mt19937();
	static boost::uniform_01<boost::mt19937> generator(*rng);
	std::vector< drunken_octo<double, double> *> nodes;

	double f = 0.0;
	for(int i = 0; i < sizeList; i++)
	{
	    f = generator();
	    Node1D *newNode = new Node1D( &f, &f );
	    nodes.push_back(newNode);
	} 

	Timer tm;
	std::ostringstream ooo;
	std::string str;

	tm.start();
	int retVal = splitList<double, double>( &nodes, &comparef, 0, nodes.size()-1, nodes.size()/2, 0);
	tm.stop();
	std::cout << sizeList << "\t" << tm.duration() << std::endl;
	sizeList *= 2;
    }
}

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
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <gsl/gsl_rng.h>

/*
 * In this example, we will create a quadrant tree spanning 
 * a two-dimensional space. Quadrant edge will be lower bound inclusive
 * higher bound exclusive. Numbering will not be the standard but:
 *  +---+---+
 *  [ 1 [ 0 [
 *  +---+---+
 *  [ 3 [ 2 [
 *  +---+---+
 */

typedef struct
{
    double x, y;
} myVector;

//int quadrant_f( drunken_octo<myVector, myVector> *parent, drunken_octo<myVector, myVector> *child );
int compare_myVector( myVector *A, myVector *B, int rank );

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

int
main(int argc, char **argv)
{
    std::vector< drunken_octo<double, double> *> nodes;
    for(int i = 0; i < 3; i++)
    {
	double f = (double) i;
	Node1D *newNode = new Node1D( &f, &f, &comparef, 1);
	nodes.push_back(newNode);
    } 
    for(std::vector<drunken_octo<double, double> *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
	double data = 0.0;
	(*it)->getData(&data);
	std::cout << data << ", ";
    }
    std::cout << std::endl;

    std::random_shuffle( nodes.begin(), nodes.end() );
    for(std::vector<drunken_octo<double, double> *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
	double data = 0.0;
	(*it)->getData(&data);
	std::cout << data << ", ";
    }
    std::cout << std::endl;

    int retVal = splitList<double, double>( &nodes, 0, nodes.size()-1, nodes.size()/2, 0);
    double f = 0.0;
    nodes.at(retVal)->getData(&f);
    std::cout << "Central point : " << f << std::endl;
    for(std::vector<drunken_octo<double, double> *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
	double data = 0.0;
	(*it)->getData(&data);
	std::cout << data << ", ";
    }
    std::cout << std::endl;

    return 0;
}

int compare_myVector( myVector *A, myVector *B, int rank )
{

    return 0;
}

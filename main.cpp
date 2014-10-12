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

typedef drunken_octo<myVector, myVector> Node2D;

int
main(int argc, char **argv)
{
    myVector rootPosition = {0.0, 0.0};
    Node2D *root = new Node2D(
	&rootPosition,
	&compare_myVector,
	2);
/*
    root->setExtents( rootExtents );
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    myVector pos;
    for (int i = 0; i < 1000000; i++) 
    {
	pos.x = gsl_rng_uniform (r);
	pos.y = gsl_rng_uniform (r);
	Node2D *node = new Node2D( &pos, &quadrant_f, 2 );
	root->addNode(node);
    }

    gsl_rng_free (r);

    std::vector< Node2D * > leaves;
    root->getLeaves( &leaves );

    for(std::vector< Node2D *>::iterator it = leaves.begin(); it != leaves.end(); ++it)
    {
	(*it)->getData(&pos);
	//std::cout << "This leaf is located at: " << pos.x << ", " << pos.y << std::endl;
    }

    delete root;
  */  
    return 0;
}

int compare_myVector( myVector *A, myVector *B, int rank )
{

    return 0;
}

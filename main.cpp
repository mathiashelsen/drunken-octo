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

typedef drunken_octo<myVector, myVector> Node2D;

int quadrant_f( drunken_octo<myVector, myVector> *parent, drunken_octo<myVector, myVector> *child )
{
    myVector childPos;
    child->getData( &childPos );
    myVector parentExtents[2];
    parent->getExtents( parentExtents );
    // Assert that the child indeed fits in the extents of the parent
    assert( (childPos.x >= parentExtents[0].x) && (childPos.x < parentExtents[1].x) );
    assert( (childPos.y >= parentExtents[0].y) && (childPos.y < parentExtents[1].y) );

    // Calculate the center of the parent node
    double centerX = (parentExtents[0].x + parentExtents[1].x)*0.5;
    double centerY = (parentExtents[0].y + parentExtents[1].y)*0.5;
   
    int quadrant_index = 0; 
    myVector childExtents[2];
    if( childPos.x < centerX )
    {
	quadrant_index += 1;
	childExtents[0].x = parentExtents[0].x;
	childExtents[1].x = centerX;
    }
    else
    {
	childExtents[0].x = centerX;
	childExtents[1].x = parentExtents[1].x;
    }

    if( childPos.y < centerY )
    {
	quadrant_index += 2;
	childExtents[0].y = parentExtents[0].y;
	childExtents[1].y = centerY;
    }
    else
    {
	childExtents[0].y = centerY;
	childExtents[1].y = parentExtents[1].y;
    }
    
    child->setExtents( childExtents );
    return quadrant_index;
}

int
main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;
    myVector rootPosition = {0.0, 0.0};
    myVector rootExtents[2];
    rootExtents[0].x = -1.0;
    rootExtents[0].y = -1.0;
    rootExtents[1].x = 1.0;
    rootExtents[1].y = 1.0;

    Node2D *root = new Node2D(
	&rootPosition,
	&quadrant_f,
	2);

    root->setExtents( rootExtents );

    myVector nodePos1 = {0.5, 0.24};
    myVector nodePos2 = {0.5, 0.23};
    Node2D *node1 = new Node2D( &nodePos1, &quadrant_f, 2 );
    Node2D *node2 = new Node2D( &nodePos2, &quadrant_f, 2 );
    root->addNode(node1);
    root->addNode(node2);

    std::vector< Node2D * > leaves;
    root->getLeaves( &leaves );

    myVector pos;
    for(std::vector< Node2D *>::iterator it = leaves.begin(); it != leaves.end(); ++it)
    {
	(*it)->getData(&pos);
	std::cout << "This leaf is located at: " << pos.x << ", " << pos.y << std::endl;
    }

    delete root;
    
    return 0;
}

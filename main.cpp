#include "drunken_octo.hpp"
#include <iostream>
#include <assert.h>

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
    myVector bla[2];
    root->getExtents( bla );
    std::cout << bla[0].x << ", " << bla[1].x << std::endl;

    delete root;
    
    return 0;
}

#include "drunken_octo.hpp"
#include <iostream>

/*
 * In this example, we will create a quadrant tree spanning 
 * a two-dimensional space.
 */

typedef struct
{
    double x, y;
} myVector;

template class drunken_octo<myVector, myVector>;


int quadrant_f( drunken_octo<myVector, myVector> *parent, drunken_octo<myVector, myVector> *child )
{
    myVector childPos;
    child->getData( &childPos );
    myVector parentExtents[2];
    parent->getExtents( parentExtents );

    return 0;
}

int
main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;
    
    return 0;
}

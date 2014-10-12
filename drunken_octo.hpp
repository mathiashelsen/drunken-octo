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

#ifndef _DRUNKEN_OCTO_HPP
#define _DRUNKEN_OCTO_HPP

#include <vector>
#include <string.h>
#include <iostream>

template <class T, class S> class drunken_octo
{
private:
    // Each drunken_octo node ofcourse needs to be able to carry some information
    T nodeData;
    // Each node possibly has extents in any of its N dimensions
    S nodePosition;
    // Each node has a parent, unless it is root (then it is NULL)
    drunken_octo<T, S> *parent;
    // Each node can have some children
    drunken_octo<T, S> *children[2];
    // The number of dimensions 
    int N;
    // The depth at which the node is located
    int depth;
    // Is this node a leaf?
    bool leaf;
    // A function pointer to compare the values of two nodes according to a specific
    // divisional hyperplane indicated by the rank
    int (* compare)( S *A, S *B, int rank);
public:
    // Adding a node requires only a datapoint, a comparison function and a function to yield the extents of a node
    drunken_octo ( 
	T *dataPoint, 
	int (* _compare)( S *A, S *B, int rank),
	int _N );
    // Remove a node, and its children!
    ~drunken_octo();
    // Add a datapoint to an existing tree
    void addNode( drunken_octo<T, S> *newNode );
    // Build an entire tree, algorithm sets the root
    void buildTree( std::vector<drunken_octo<T, S> *> nodeList, drunken_octo<T, S> *root );
    // Get the data from a specific node
    void getData( T *data );
    // Get the position of a specific node
    void getPosition( S *position );
    // Retrieves a vector/list of pointers to nodes that are leaves of the tree
    void getLeaves( std::vector<drunken_octo<T, S> *> *leavesList );
    // Retrieve the N closest leaves to a specific point, where a metric function is specified.
    //void getNeighbours( S *position, double ( * metric )( S *a, S *b), int N, std::vector<drunken_octo<T, S> *> *leavesList);
};

template <class T, class S> sortNodelist( 

template <class T, class S> drunken_octo<T, S>::drunken_octo ( 
	T *dataPoint, 
	int (* _compare)( S *A, S *B, int rank),
	int _N )
{
    memcpy( &nodeData, dataPoint, sizeof(T) );
    compare = _compare;
    N = _N;
    depth = 0;
    leaf = true;

    children[0] = NULL;
    children[1] = NULL;
}

template <class T, class S> drunken_octo<T, S>::~drunken_octo()
{
    if(children[0] != NULL)
    {
	delete children[0];
    }
    if(children[1] != NULL)
    {
	delete children[1];
    }

    delete[] children;
}

template <class T, class S> void drunken_octo<T, S>::addNode( drunken_octo<T, S> *newNode )
{
    int leq = compare( this, newNode );
    if( leq == -1 )
    {
	if(children[0] == NULL)
	{
	    children[0] = newNode;
	}
	else
	{
	    children[0].addNode( newNode );
	}
    }
    else
    {
	if(children[1] == NULL)
	{
	    children[1] = newNode;
	}
	else
	{
	    children[1].addNode(newNode);
	}
    }

    leaf = false;
}

template <class T, class S> void drunken_octo<T, S>::getData( T *data )
{
    memcpy(data, &nodeData, sizeof(T));
}

template <class T, class S> void drunken_octo<T, S>::getPosition( S *position )
{
    memcpy(position, &nodePosition, sizeof(S));
}


template <class T, class S> void drunken_octo<T, S>::getLeaves( std::vector<drunken_octo<T, S> *> *leavesList )
{
    if(leaf)
    {
	leavesList->push_back(this);
    }
    else
    {
	for(int i = 0; i < 2; i++)
	{
	    if(children[i] != NULL)
	    {
		children[i]->getLeaves( leavesList );
	    }
	}
    }
}

#endif

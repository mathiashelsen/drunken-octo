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
    S nodeExtents[2];
    // Each node has a parent, unless it is root (then it is NULL)
    drunken_octo<T, S> *parent;
    // Each node can have some children
    drunken_octo<T, S> **children;
    // The number of dimensions 
    int N;
    int nChildren;
    // Is this node a leaf?
    bool leaf;
    // The function to determine at which quadrant/octant/... a child belongs
    // and set the new extents of the new node
    int (* tant)( drunken_octo<T, S> *parent, drunken_octo<T, S> *child );
public:
    // Adding a node requires only a datapoint, a comparison function and a function to yield the extents of a node
    drunken_octo ( 
	T *dataPoint, 
	int (* _tant)( drunken_octo<T, S> *parent, drunken_octo<T, S> *child ),
	int _N );
    // Remove a node, and its children!
    ~drunken_octo();
    // Add a datapoint to an existing tree
    void addNode( drunken_octo<T, S> *newNode );
    void getExtents( S *extents );
    void setExtents( S *extents );
    void getData( T *data );
    // Retrieves a vector/list of pointers to nodes that are leaves of the tree
    void getLeaves( std::vector<drunken_octo<T, S> *> *leavesList );
    // Retrieve the N closest leaves to a specific point, where a metric function is specified.
    void getNeighbours( S *position, double ( * metric )( S *a, S *b), int N, std::vector<drunken_octo<T, S> *> *leavesList);
};

template <class T, class S> drunken_octo<T, S>::drunken_octo ( 
	T *dataPoint, 
	int (* _tant)( drunken_octo<T, S> *parent, drunken_octo<T, S> *child ),
	int _N )
{
    memcpy( &nodeData, dataPoint, sizeof(T) );
    tant = _tant;
    N = _N;
    nChildren = 1;
    leaf = true;
    for(int i = 0; i < N; i++)
    {
	nChildren *= 2;
    }

    children = new drunken_octo<T, S> *[nChildren];
    for(int i = 0; i < nChildren; i++ )
    {
	children[i] = NULL;
    }
}

template <class T, class S> drunken_octo<T, S>::~drunken_octo()
{
    for(int i = 0; i < nChildren; i++)
    {
	if(children[i] != NULL)
	{
	    delete children[i];
	}
    }
    delete[] children;
}

template <class T, class S> void drunken_octo<T, S>::addNode( drunken_octo<T, S> *newNode )
{
    int tantIndex = tant( this, newNode );
    if( children[tantIndex] != NULL )
    {
	children[tantIndex]->addNode(newNode);
    }
    else
    {
	children[tantIndex] = newNode;
    }

    if( leaf )
    {
	leaf = false;
    }
}

template <class T, class S> void drunken_octo<T, S>::getExtents( S *extents )
{
    memcpy( extents, nodeExtents, sizeof(S)*2);
}

template <class T, class S> void drunken_octo<T, S>::setExtents( S *extents )
{
    memcpy( nodeExtents, extents, sizeof(S)*2);
}

template <class T, class S> void drunken_octo<T, S>::getData( T *data )
{
    memcpy(data, &nodeData, sizeof(T));
}

template <class T, class S> void drunken_octo<T, S>::getLeaves( std::vector<drunken_octo<T, S> *> *leavesList )
{
    if(leaf)
    {
	leavesList->push_back(this);
    }
    else
    {
	for(int i = 0; i < nChildren; i++)
	{
	    if(children[i] != NULL)
	    {
		children[i]->getLeaves( leavesList );
	    }
	}
    }
}

#endif

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
#include <algorithm>

template <class T, class S> class drunken_octo
{
private:
    // Each drunken_octo node ofcourse needs to be able to carry some information
    T nodeData;
    // Each node possibly has extents in any of its N dimensions
    S nodePosition;
    // Each node has a parent, unless it is root (then it is NULL)
    drunken_octo<T, S> *parent;
    // The depth at which the node is located
    int depth;
    // Is this node a leaf?
    bool leaf;
public:
    // Each node can have some children
    drunken_octo<T, S> *children[2];
    // Adding a node requires only a datapoint, a comparison function and a function to yield the extents of a node
    drunken_octo (T *dataPoint, S *position);
    // Remove a node, and its children!
    ~drunken_octo();
    // Add a datapoint to an existing tree
    void addNode( 
	drunken_octo<T, S> *newNode, 
	int (* compare)( S *A, S *B, int rank),
	int k,
	int depth );
    // Build an entire tree, algorithm sets the root
    void buildTree( 
	std::vector<drunken_octo<T, S> *> nodeList, 
	drunken_octo<T, S> **parent,
	int (* compare)( S *A, S *B, int rank),
	int k,
	int depth,
	int left,
	int right );
    // Get the data from a specific node
    T* getData() { return &nodeData; };
    // Get the position of a specific node
    S *getPosition( ){ return &nodePosition; };
    void setDepth ( int _depth );
    // Retrieves a vector/list of pointers to nodes that are leaves of the tree
    void getLeaves( std::vector<drunken_octo<T, S> *> *leavesList );
    // Compares a node with another one
    // Retrieve the N closest leaves to a specific point, where a metric function is specified.
    void findNN( 
	drunken_octo<T, S>** currentBest,
	double currentDist,
	S *target,
	double (* metricFunc)( S *target, S *current)
	);
    //void getNeighbours( S *position, double ( * metric )( S *a, S *b), int N, std::vector<drunken_octo<T, S> *> *leavesList);
};

template <class T, class S> static int partition(
    std::vector< drunken_octo<T, S> *> *input, 
    int (* compare)( S *A, S *B, int rank),
    int _left,
    int _right,
    int _pivot,
    int rank)
{
    drunken_octo<T, S> *pivot = input->at(_pivot);
    std::swap(input->at(_pivot), input->at(_right));
    int si = _left;
    for( int i = _left; i < _right; i++ )
    {
	if( compare( input->at(i)->getPosition(), pivot->getPosition(), rank) <= 0 )
	{
	    std::swap(input->at(i), input->at(si));
	    si++;
	}
    }
    std::swap(input->at(_right), input->at(si));
    return si;
}

template <class T, class S> int splitList( 
    std::vector< drunken_octo<T, S> *> *input, 
    int (* compare)( S *A, S *B, int rank),
    int left,
    int right,
    int target,
    int rank)
{
    int pivotIndex = (left + right) / 2;
    if( right == left ) return pivotIndex;
    pivotIndex = partition( input, compare, left, right, pivotIndex, rank);
    if( pivotIndex == target )
    {
	return pivotIndex;
    }
    else if(pivotIndex > target)
    {
	return splitList(input, compare, left, pivotIndex-1, target, rank);
    }
    else
    {
	return splitList(input, compare, pivotIndex+1, right, target, rank);
    }
}

template <class T, class S> drunken_octo<T, S>::drunken_octo ( 
	T *dataPoint, 
	S *position)
{
    memcpy( &nodeData, dataPoint, sizeof(T) );
    memcpy( &nodePosition, position, sizeof(S) );
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

    //delete[] children;
}

template <class T, class S> void buildTree( 
	std::vector<drunken_octo<T, S> *> *nodeList, 
	drunken_octo<T, S> **parent,
	int (* compare)( S *A, S *B, int rank),
	int k,
	int _depth)
{
    if( nodeList->size() > 0 )
    {
	int retVal = splitList( 
	    nodeList,
	    compare,
	    0,
	    nodeList->size() - 1,
	    nodeList->size() / 2,
	    _depth%k); 	

	*parent = nodeList->at(retVal);
	(*parent)->setDepth(_depth);
	if( nodeList->size() > 1 )
	{
	    std::vector<drunken_octo<T, S> *> left(nodeList->begin(), nodeList->begin() + retVal);
	    std::vector<drunken_octo<T, S> *> right(nodeList->begin() + retVal + 1, nodeList->end());
	    if( left.size() > 0 )
	    {
		buildTree( &left, &((*parent)->children[0]), compare, k, _depth+1);
	    }
	    if( right.size() > 0 )
	    {
		buildTree( &right, &((*parent)->children[1]), compare, k, _depth+1);
	    }
	}
    }
}

template <class T, class S> void drunken_octo<T, S>::addNode(
	drunken_octo<T, S> *newNode, 
	int (* compare)( S *A, S *B, int rank),
	int k,
	int depth )
{
    int leq = compare( this->getPosition(), newNode->getPosition, depth%k );	
    int i = (leq == -1) ? 0 : 1;
    if(children[i] == NULL)
    {
        children[i] = newNode;
	newNode->setDepth ( depth );
    }
    else
    {
	children[i].addNode( 
	    newNode, 
	    compare,
	    k,
	    depth+1 );
    }

    leaf = false;
}

template <class T, class S> void drunken_octo<T, S>::setDepth( int _depth )
{
    depth = _depth;
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

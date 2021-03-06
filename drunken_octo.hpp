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

#define FORK_THREAD_DEPTH 2

#include <vector>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "sorted_ll.hpp"

typedef struct
{
    int		    maxThreads;
    int		    curThreads;
    pthread_mutex_t lock;
    
} threadCtrl;

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
	threadCtrl *mgr
	);
    // Get the data from a specific node
    T* getData() { return &nodeData; };
    // Get the position of a specific node
    S *getPosition( ){ return &nodePosition; };
    void setDepth ( int _depth );
    // Retrieves a vector/list of pointers to nodes that are leaves of the tree
    void getLeaves( std::vector<drunken_octo<T, S> *> *leavesList );
    // Compares a node with another one
    // Retrieve the closest leaf to a specific point, where a metric function is specified.
    void findNN( 
	drunken_octo<T, S>** currentBest,
	double *currentDist,
	S *target,
	double (* metricFunc)( S *target, S *current),
	double (* projectedDistance)( S* target, S *current, int rank),
	int k,
	threadCtrl *mgr );
    // Retrieve the closest N leaves to a specific point, where a metric function is specified.
    void findNN(
	Sorted_LL< drunken_octo<T, S> > *NN,
	S *target,
	double (* metricFunc)( S *target, S *current),
	double (* projectedDistance)( S* target, S *current, int rank),
	int k,
	int numNeighbours );
	
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
}

template <class T, class S> struct find_NN_t_args
{
    drunken_octo<T, S> *node;
    drunken_octo<T, S>** currentBest;
    double *currentDist;
    S *target;
    double (* metricFunc)( S *target, S *current);
    double (* projectedDistance)( S* target, S *current, int rank);
    int k;
    threadCtrl *mgr;
};

template <class T, class S> void *findNN_t( void *args )
{
    find_NN_t_args<T, S> *ptr = (find_NN_t_args<T, S> *) args;
    //std::cout << "# New thread " << pthread_self() << " launched" << std::endl;
    (ptr->node)->findNN(
	ptr->currentBest,
	ptr->currentDist,
	ptr->target,
	ptr->metricFunc,
	ptr->projectedDistance,
	ptr->k,
	ptr->mgr
	);
    //std::cout << "# Thread " << pthread_self() << " finished" << std::endl;
    pthread_exit(NULL);
    return NULL;
}

template <class T, class S> void drunken_octo<T, S>::findNN( 
	drunken_octo<T, S>** currentBest,
	double *currentDist,
	S *target,
	double (* metricFunc)( S *target, S *current),
	double (* projectedDistance)( S* target, S *current, int rank),
	int k,
	threadCtrl *mgr
	)
{
    // Check if the current node is closer than the previous best estimate
    double dist = metricFunc( target, &nodePosition );
    //pthread_mutex_lock(&(mgr->lock));
    if( dist < *currentDist )
    {	
	*currentBest = this;
	*currentDist = dist;
    }
    //pthread_mutex_unlock(&(mgr->lock));

    // Can some of the points on the left seperating hyperplane provide a better match?
    dist = projectedDistance( target, &nodePosition, depth%k );

    bool addedThread = false;
    pthread_t daughterThread;
    
    // Make sure the target doesn't lie on the hyperplane
    if( dist != 0.0 )
    {
	int i = ( dist < 0.0 ) ? 0 : 1;
	if(children[i] != NULL)
	{
		children[i]->findNN(currentBest, currentDist, target, metricFunc, projectedDistance, k, mgr);
	}
	// Is the can it lie on other side of the seperating hyperplane?
	double currentDistAlias;
//	pthread_mutex_lock(&(mgr->lock));
	currentDistAlias = *currentDist;
//	pthread_mutex_unlock(&(mgr->lock));

	if( fabs(dist) < currentDistAlias )
	{
//	    pthread_mutex_unlock(&(mgr->lock));
	    if(children[1-i] != NULL)
	    {
/*		pthread_mutex_lock(&(mgr->lock));
		if( mgr->curThreads < mgr->maxThreads )
		{
//		    std::cout << "Forking thread" << std::endl;
		    // Fork a new thread
		    struct find_NN_t_args<T, S> threadArgs;
		    threadArgs.node = children[1-i];
		    threadArgs.currentBest = currentBest;
		    threadArgs.currentDist = currentDist;
		    threadArgs.target = target;
		    threadArgs.metricFunc = metricFunc;
		    threadArgs.projectedDistance = projectedDistance;
		    threadArgs.k = k;
		    threadArgs.mgr = mgr;

		    pthread_create( &daughterThread, NULL, findNN_t<T, S>, (void *) &threadArgs );
		    mgr->curThreads++;
		    addedThread = true;

		    // Unlock the mutex
		    pthread_mutex_unlock(&(mgr->lock));
		}
		else
		{
		    pthread_mutex_unlock(&(mgr->lock));*/
		children[1-i]->findNN(currentBest, currentDist, target, metricFunc, projectedDistance, k, mgr);
		//}
	    }
	}
    }
    else
    {
	if(children[0] != NULL)
	{
	    children[0]->findNN(currentBest, currentDist, target, metricFunc, projectedDistance, k, mgr);
	}
	if(children[1] != NULL)
	{
	    children[1]->findNN(currentBest, currentDist, target, metricFunc, projectedDistance, k, mgr);
	}
    }
    
    if(addedThread)
    {
	pthread_join( daughterThread, NULL );
	pthread_mutex_lock(&(mgr->lock));
	mgr->curThreads--;
	pthread_mutex_unlock(&(mgr->lock));
    }
}

/*
template <class T, class S> struct findNN_t_args_struct
{
    drunken_octo<T, S> *object;
    Sorted_LL< drunken_octo<T, S> > *NN;
    S *target;
    double (* metricFunc)( S *target, S *current);
    double (* projectedDistance)( S* target, S *current, int rank);
    int k;
    int numNeighbours;

};

template <class T, class S> void * findNN_t( void *args )
{
    printf("Created new thread, 
    struct findNN_t_args_struct<T, S> *ptr = (struct findNN_t_args_struct<T, S> *)args;
    (ptr->object).findNN( ptr->NN, ptr->target, ptr->metricFinc, ptr->projectedDistance, ptr->k, ptr->numNeighbours );
    return NULL;
}
*/

template <class T, class S> void drunken_octo<T, S>::findNN(
	Sorted_LL< drunken_octo<T, S> > *NN,
	S *target,
	double (* metricFunc)( S *target, S *current),
	double (* projectedDistance)( S* target, S *current, int rank),
	int k,
	int numNeighbours )
{
    // Check if the current node is closer than the previous best estimate
    double dist = metricFunc( target, &nodePosition );
    if( (dist < NN->getMax()) || (numNeighbours > NN->getElements()) )
    {	
	NN->insert(this, dist);
    }

    // Can some of the points on the left seperating hyperplane provide a better match?
    dist = projectedDistance( target, &nodePosition, depth%k );
    
    // Make sure the target doesn't lie on the hyperplane
    if( dist != 0.0 )
    {
	int i = ( dist < 0.0 ) ? 0 : 1;
	if(children[i] != NULL)
	{
	    children[i]->findNN(NN, target, metricFunc, projectedDistance, k, numNeighbours);
	}
	// Can it lie on other side of the seperating hyperplane? (or aren't there enough datapoints)
	if( (fabs(dist) < NN->getMax()) || (numNeighbours > NN->getElements()) )
	{
	    if(children[1-i] != NULL)
	    {
		children[1-i]->findNN(NN, target, metricFunc, projectedDistance, k, numNeighbours);
	    }
	}	

    }
    else
    {
	if(children[0] != NULL)
	{
	    children[0]->findNN(NN, target, metricFunc, projectedDistance, k, numNeighbours);
	}
	if(children[1] != NULL)
	{
	    children[1]->findNN(NN, target, metricFunc, projectedDistance, k, numNeighbours);
	}
    }

}

template <class T, class S> struct buildTree_t_args
{
    std::vector<drunken_octo<T, S> *> *nodeList;
    drunken_octo<T, S> **parent;
    int (* compare)( S *A, S *B, int rank);
    int k;
    int _depth;
    threadCtrl *mgr;
};

template <class T, class S> void *buildTree_t ( void *args )
{
    struct buildTree_t_args<T, S> *ptr = (struct buildTree_t_args<T, S> *) args;
    //std::cout << "# New thread " << pthread_self() << " launched at depth " << ptr->_depth << std::endl;
    buildTree(ptr->nodeList, ptr->parent, ptr->compare, ptr->k, ptr->_depth, ptr->mgr);
    //std::cout << "# Thread " << pthread_self() << " finished" << std::endl;
    pthread_exit(NULL);
}

template <class T, class S> void buildTree( 
	std::vector<drunken_octo<T, S> *> *nodeList, 
	drunken_octo<T, S> **parent,
	int (* compare)( S *A, S *B, int rank),
	int k,
	int _depth,
	threadCtrl *mgr)
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
	    bool threadAdded = false;
	    pthread_t daughterThread;

	    std::vector<drunken_octo<T, S> *> left(nodeList->begin(), nodeList->begin() + retVal);
	    std::vector<drunken_octo<T, S> *> right(nodeList->begin() + retVal + 1, nodeList->end());
    
	    if( left.size() > 0 )
	    {
		pthread_mutex_lock( &(mgr->lock) );
		if( mgr->curThreads < mgr->maxThreads )
		{
		    // Add a new thread
		    struct buildTree_t_args<T, S> args;
		    args.nodeList = &left;
		    args.parent = &((*parent)->children[0]);
		    args.compare = compare;
		    args.k = k;
		    args._depth = _depth+1;
		    args.mgr = mgr;
		    pthread_create( &daughterThread, NULL, buildTree_t<T, S>, (void *)&args );
		    threadAdded = true;
		    mgr->curThreads++;
		    pthread_mutex_unlock( &(mgr->lock) );
		}
		else
		{
		    pthread_mutex_unlock( &(mgr->lock) );
		    buildTree( &left, &((*parent)->children[0]), compare, k, _depth+1, mgr);
		}
	    }
		
	    if( right.size() > 0 )
	    {
		pthread_mutex_lock( &(mgr->lock) );
		if( (mgr->curThreads < mgr->maxThreads) && !threadAdded )
		{
		    // Add a new thread
		    struct buildTree_t_args<T, S> args;
		    args.nodeList = &right;
		    args.parent = &((*parent)->children[1]);
		    args.compare = compare;
		    args.k = k;
		    args._depth = _depth+1;
		    args.mgr = mgr;
		    pthread_create( &daughterThread, NULL, buildTree_t<T, S>, (void *)&args );
		    threadAdded = true;
		    mgr->curThreads++;
		    pthread_mutex_unlock( &(mgr->lock) );
		}
		else
		{
		    pthread_mutex_unlock( &(mgr->lock) );
		    buildTree( &right, &((*parent)->children[1]), compare, k, _depth+1, mgr);
		}
	    }

	    // If we spawned a new thread, we should wait until it finishes
	    if( threadAdded )
	    {
		pthread_join( daughterThread, NULL );
		pthread_mutex_lock(&(mgr->lock));
		mgr->curThreads--;
		pthread_mutex_unlock(&(mgr->lock));
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

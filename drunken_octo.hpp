#ifndef _DRUNKEN_OCTO_HPP
#define _DRUNKEN_OCTO_HPP

#include <vector>
#include <string.h>

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
    for(int i = 0; i < N; i++)
    {
	nChildren *= 2;
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

#endif

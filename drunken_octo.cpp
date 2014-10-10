#include "drunken_octo.hpp"

template <class T> drunken_octo<T>::drunken_octo ( 
	T *dataPoint, 
	int (* _tant)( T *parent, T *child), 
	void (* _extents)( T *parent, T *child, T **nodeExtents),
	int _N );
{
    nodeData = *dataPoint;
    tant = _tant;
    extents = _extents;
    N = _N;
}

template <class T> drunken_octo<T>::~drunken_octo()
{
    for(int i = 0; i < N; i++)
    {
	if(children[i] != NULL)
	{
	    delete children[i];
	}
    }
}

template <class T> void drunken_octo<T>::addNode( drunken_octo<T> *newNode )
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

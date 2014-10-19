#ifndef _SORTED_LL_HPP
#define _SORTED_LL_HPP

#include <assert.h>

template <typename T> struct list_elem
{
    T* Node;
    double dist;
    list_elem<T> *nextElem;
    list_elem<T> *prevElem;
} ;

template <class T> class Sorted_LL
{
private:
    list_elem<T> *root;
    list_elem<T> *curr;
    double max;
    int nElems;
    int maxElems;
public:
    Sorted_LL( int _maxElems );
    void insert( T* _Node, double _dist );
    void getNext( T **_Node, double *_dist);
    double getMax(){ return max; };
    int getElements(){ return nElems; };
};

template <class T> Sorted_LL<T>::Sorted_LL( int _maxElems )
{
    maxElems = _maxElems;
    max = 0.0;
    nElems = 0;
    root = NULL;
    curr = NULL;
}

template <class T> void Sorted_LL<T>::insert( T* _Node, double _dist )
{
    if( (_dist < max) || (nElems < maxElems) )
    {
	list_elem<T> *newElem = new list_elem<T>;
	newElem->Node = _Node;
	newElem->dist = _dist;
	list_elem<T> *it = root;
	// If root is not null
	if( it != NULL )
	{
	    // Keep iterating over the elements until it is either the last
	    // or it's value is larger than our new value
	    while( (it->nextElem != NULL) && (it->dist < newElem->dist) )
	    {
		it = it->nextElem;
	    }

	    // Append before if the iterator value is larger than the new value
	    if( it->dist > newElem->dist )
	    {
		newElem->prevElem = it->prevElem;
		newElem->nextElem = it;
		if(newElem->prevElem != NULL) newElem->prevElem->nextElem = newElem;
		if(newElem->nextElem != NULL) newElem->nextElem->prevElem = newElem;
		if( it == root ) root = newElem;
		nElems++;
	    }
	    // append after if it was the last value on the list
	    else
	    {
		newElem->prevElem = it;
		newElem->nextElem = NULL;
		newElem->prevElem->nextElem = newElem;
		nElems++;
	    }

	    // Perhaps we have to update the maximum value
	    if( newElem->dist > max )
	    {
		max = newElem->dist;
	    }
    
	    // If we made the list longer than allowed
	    if( nElems > maxElems )
	    {
		it = root;
		while( it->nextElem->nextElem != NULL )
		{
		    it = it->nextElem;
		}
		delete it->nextElem;
		it->nextElem = NULL;
		max = it->dist;
		nElems--;
	    }
	    
	}
	// The new node will be root
	else
	{
	    root = newElem;
	    newElem->nextElem = NULL;
	    newElem->prevElem = NULL;
	    nElems++;
	    max = newElem->dist;
	}
    }
}
template <class T> void Sorted_LL<T>::getNext(T **_Node, double *_dist)
{
    if( curr == NULL )
    {
	curr = root;
    }
    else
    {
	curr = curr->nextElem;
    }	

    if( curr != NULL )
    {
	*_Node = curr->Node;
	*_dist = curr->dist;
    }
    else
    {
	*_Node = NULL;
	*_dist = 0;
    }
}

#endif

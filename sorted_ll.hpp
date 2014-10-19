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
    T* getNext();
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
	if( it != NULL )
	{
	    // Keep iterating over the elements until it is either the last
	    // or it's value is larger than our new value
	    while( (it->nextElem != NULL) && (it->dist < newElem->dist) )
	    {
		it = it->nextElem;
	    }

	    // Append before...
	    if( it->dist > newElem->dist )
	    {
		std::cout << "Inserting " << *(newElem->Node) << " before " << *(it->Node) << std::endl;
		newElem->prevElem = it->prevElem;
		newElem->nextElem = it;
		if(newElem->prevElem != NULL) newElem->prevElem->nextElem = newElem;
		if(newElem->nextElem != NULL) newElem->nextElem->prevElem = newElem;
		if( it == root ) root = newElem;
		nElems++;
	    }
	    // append after...
	    else
	    {
		std::cout << "Inserting " << *(newElem->Node) << " after " << *(it->Node) << std::endl;
		newElem->prevElem = it;
		newElem->nextElem = NULL;
		newElem->prevElem->nextElem = newElem;
		nElems++;
	    }

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
	else
	{
	    std::cout << "Setting " << *(newElem->Node) << " as root" << std::endl;
	    root = newElem;
	    newElem->nextElem = NULL;
	    newElem->prevElem = NULL;
	    nElems++;
	    max = newElem->dist;
	}
    }
}
template <class T> T* Sorted_LL<T>::getNext()
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
	return curr->Node;
    }
    else
    {
	return NULL;
    }
}

#endif

#ifndef _SORTED_LL_HPP
#define _SORTED_LL_HPP

template <typename T> struct list_elem
{
    T* Node;
    double dist;
    list_elem<T> *nextElem;
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
    std::cout << "Adding node " << *_Node << " at distance " << _dist << std::endl;
    if( (_dist < max) || (nElems < maxElems) )
    {
	list_elem<T> *newElem = new list_elem<T>;
	newElem->Node = _Node;
	newElem->dist = _dist;
	if( _dist > max )
	{
	    max = _dist;
	}

	if( root == NULL )
	{
	    root = newElem;
	    root->nextElem = NULL;
	    nElems++;
	}
	else
	{
	    list_elem<T> *it = root; 
	    while( (it->nextElem != NULL) && (it->dist < _dist) )
	    {
		it = it->nextElem;
	    }

	    //Append to the end of the list if it is short enough
	    if( (it->nextElem == NULL) && (nElems < maxElems) )
	    {
		it->nextElem = newElem;
		newElem->nextElem = NULL;
		nElems++;
	    }

	    //Insert somewhere in the list
	    else if( it->nextElem != NULL )
	    {
		list_elem<T> *tmp = it->nextElem;
		it->nextElem = newElem;
		newElem->nextElem = tmp;

		nElems++;

		// Check if the list didn't grow out of proportion
		if( nElems > maxElems )
		{
		    // Delete the last element of the list
		    while( it->nextElem->nextElem != NULL )
		    {
			it = it->nextElem;
		    }
		    delete it->nextElem;
		    it->nextElem = NULL;
		}
	    }
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

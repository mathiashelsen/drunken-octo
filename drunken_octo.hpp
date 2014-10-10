#ifndef _DRUNKEN_OCTO_HPP
#define _DRUNKEN_OCTO_HPP

#include <vector>
#define NULL 0

template <class T> class drunken_octo
{
private:
    // Each drunken_octo node ofcourse needs to be able to carry some information
    T nodeData;
    // Each node possibly has extents in any of its N dimensions
    T *nodeExtents;
    // Each node has a parent, unless it is root (then it is NULL)
    drunken_octo<T> *parent;
    // Each node can have some children
    drunken_octo<T> *children;
    // The number of dimensions
    int N;
    // and the resulting number of children (2**N)
    int nChildren;
    // The function to determine at which quadrant/octant/... a child belongs
    int (* tant)( T *parent, T *child );
    // The function to calculate the extents of each node
    void (* extents)( T *parent, T *child, T **nodeExtents);
public:
    // Adding a node requires only a datapoint, a comparison function and a function to yield the extents of a node
    drunken_octo ( T *dataPoint, 
	int (* _tant)( T *parent, T *child), 
	void (* _extents)( T *parent, T *child, T **nodeExtents),
	int _N );
    // Remove a node, and its children!
    ~drunken_octo();
    // Add a datapoint to an existing tree
    void addNode( drunken_octo<T> *newNode );
};

#endif

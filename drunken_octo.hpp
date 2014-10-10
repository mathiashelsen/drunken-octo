#ifndef _DRUNKEN_OCTO_HPP
#define _DRUNKEN_OCTO_HPP

#include <vector>

template <class T> class drunken_octo
{
private:
    // Each drunken_octo node ofcourse needs to be able to carry some information
    T nodeData;
    // Each node has a parent, unless it is root (then it is NULL)
    drunken_octo<T> *parent;
    // Each node can have some children
    drunken_octo<T> *children;
    // The maximum number of children
    int N;
    // The function to determine at which quadrant/octant/... a child belongs
    int (* tant)( T *parent, T *child );
public:
    drunken_octo ( T *dataPoint, int (* _tant)( T *parent, T *child) );
    ~drunken_octo();
    // Check if the node is a root node
    bool IsRoot();  
    // Add a datapoint to an existing tree
    void addNode( drunken_octo<T> *newNode, drunken_octo<T> *parent );
};

#endif

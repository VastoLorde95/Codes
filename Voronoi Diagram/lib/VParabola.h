/*
    Class description
    
    This class represents the parabolas of the beach line, but for all practical purposes it actually represents the nodes of the binary search tree of the beach line and this class implements all functions to implement the beach line. Internal nodes of the binary search tree represent edges and the leaf nodes of the binary search tree represent the piece-wise parabolas
    
    We must note that Parabolas and Events point to each other since for each parabola we must know which circle event it corresponds to (sp that we may delete this events if they become invalid) and which parabola must be deleted for each circle event.
    
    public attributes:
    
    isLeaf      : flag whether the node is a leaf node or an internal node (i.e. an edge or a parabola)
    site        : pointer to the focus of the parabola (if it is a parabola, NULL otherwise)
    edge        : pointer to the edge (when it is an internal node, NULL otherwise)
    cEvent      : pointer to the event, when the parabola disappears, i.e., the circle events
    parent      : pointer to the parent of this node in the BST. If the node is the root, the parent is NULL
    
    public methods    
    
    The class has an overloaded constructor.
    The class constructor takes no arguments if we are constructing an edge and takes as argument the focus of the parabola if we are constructing a parabola.
    
    SetLeft     : sets the left child of the node to the given node
    SetRight    : set the right child of the node to the given node
    Left        : returns pointer to the left child of the node
    Right       : returns pointer to the right child of the node

    Tree traversal utility functions
    GetLeftSeparator    : returns the ancestor which separates the parabola from its left neighbour
    GetRightSeparator   : returns the ancestor which separates the parabola from its right neighbour
    GetLeftChild        : returns the rightmost leave which is on the left of current node
    GetRightChild       : returns the leftmost leave which is on the right of current node
    
    private attributes:
    _left        : the left child of this node. NULL if it doesn't exist
    _right       : the right child of this node. NULL if it doesn't exist
        
*/

#ifndef VParabola_h
#define VParabola_h

#include "VPoint.h"
#include "VEdge.h"

// forward declaration of class VEvent since both classes point to each other
class VEvent;

class VParabola{
public:
    bool isLeaf;
    VPoint *site;
    VEdge *edge;
    VEvent *cEvent;
    VParabola *parent;
    
    VParabola();
    VParabola(VPoint * s);

    void SetLeft(VParabola *p);
    void SetRight(VParabola *p);
    VParabola *Left ();
    VParabola *Right();
    
    // implemented in VParabola.cpp
    static VParabola * GetLeftSeparator(VParabola * p);
    static VParabola * GetRightSeparator(VParabola * p);
    static VParabola * GetLeftChild(VParabola * p);
    static VParabola * GetRightChild(VParabola * p);
    
private:
    VParabola * _left;
    VParabola * _right;
};

#endif

/*
    Class Description
    A class that stores an edge in Voronoi diagram. This corresponds to the half edges in the doubly-connected-edge list data structure which is used to represent planar graphs.
    There is not implementation for faces from DCEL since we don't need any operations over faces under general assumption. Instead, the half edges just store a pointer to the site that is responsible for generating the Voronoi face adjacent to this edge.

    public attributes:
    start       : pointer to start point
    end         : pointer to end point
    left        : pointer to Voronoi site on the left side of edge
    right       : pointer to Voronoi site on the right side of edge

    twin        : the twin edge for this half edge

    direction   : the half edge is the perpendicular bisector of left and right hence the direction is perpendicular to the line connecting left to right. The point is treated as a directional vector
    m, c        : the edge co-incides with the line given by the equation y = m * x + c
    
    public methods:
    VEdge       : class constructor. It takes the following parameters as input
    
    s           : pointer to start
    a           : pointer to left site
    b           : pointer to right site
    
    Since this is just a container class, we don't implement any private methods
*/

#ifndef VEdge_h
#define VEdge_h

#include "VPoint.h"
#include <iostream>

class VEdge{
public:
    VPoint * start;
    VPoint * end;
    VPoint * direction;
    VPoint * left;
    VPoint * right;
    
    double        m;
    double        c;

    VEdge * twin;

    VEdge(VPoint * s, VPoint * a, VPoint * b){
        start = s,
        left = a;
        right = b;
        twin = NULL;
        end = NULL;
        
        // m = -(x2-x1) / (y2-y1) = (x1-x2) / (y2-y1)
        m = (b->x - a->x) / (a->y - b->y) ;
        c = s->y - m * s->x ;
        direction = new VPoint(b->y - a->y, -(b->x - a->x));
    }

    ~VEdge(){
        delete direction ;
    }
};

#endif

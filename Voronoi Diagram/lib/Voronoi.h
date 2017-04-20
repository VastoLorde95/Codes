/*
    Class Description
    This class is used to generate the Voronoi diagram
    
    This class has only one public function to generate the diagram, i.e., GetEdges()
    
    GetEdges has the following constructor parameters:
    
    v        : The sites that generate the Voronoi diagram
    w        : width  of the bounding box
    h        : height of the bounding box
    
    The function returns a pointer to the list of Voronoi edges. The Voronoi vertices can be recovered from the start and end
    points of the Voronoi edges
    
    The class contains the following private attributes and functions
    
    private attributes:
    
    places      : container of places with which we work
    edges       : container of edges which will be teh result
    width       : width of the diagram
    height      : height of the diagram
    root        : the root of the tree, that represents a beachline sequence
    sweepline   : current "y" position of the line (see Fortune's algorithm)
    
    deleted     : set  of deleted (false) events (since we can not delete from STL priority_queue, we just check if the event exists in deleted or not)
    points      : list of all Voronoi vertices that were created during the algorithm
    queue       : priority queue with events to process
    
    private methods:
    
    InsertParabola         : this processes point events and inserts a new parabola into the BST
    RemoveParabola         : this processes circle events and deletes the corresponding parabola from the BST
    FinishEdge             : recursively finishes all unbounded edges from the BST
    GetXOfEdge             : returns the current x position of an intersection point of left and right parabolas
    GetParabolaByX         : returns the parabola that is just above the current point
    CheckCircle            : check for circle events
    GetEdgeIntersection    : returns the point of intersection of two Voronoi edges
    
*/

#ifndef Voronoi_h
#define Voronoi_h

#include<bits/stdc++.h>

#include "VPoint.h"
#include "VEdge.h"
#include "VParabola.h"
#include "VEvent.h"

using namespace std;

// short cut to save typing
typedef list<VPoint *> Vertices;
typedef list<VEdge *> Edges;

class Voronoi{
public:
    Voronoi();
    Edges *    GetEdges(Vertices * v, int w, int h);
private:
    Vertices * places;
    Edges * edges;
    double width, height;
    VParabola * root;
    double sweepline;
    
    std::set<VEvent *>    deleted;
    std::list<VPoint *> points;
    
    // largest point is on top in the priority_queue
    // thus the sweepline moves from top to bottom
    std::priority_queue<VEvent *, std::vector<VEvent *>, VEvent::CompareEvent> queue;

    void InsertParabola(VPoint * p);
    void RemoveParabola(VEvent * e);
    void FinishEdge(VParabola * n);
    double GetXOfEdge(VParabola * par, double y);
    VParabola * GetParabolaByX(double xx);
    double GetY(VPoint * p, double x);
    void CheckCircle(VParabola * b);
    VPoint * GetEdgeIntersection(VEdge * a, VEdge * b);
};

#endif

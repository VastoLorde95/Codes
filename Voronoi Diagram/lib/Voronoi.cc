/*
    Implementation of the Voronoi class. For class description refer Voronoi.h
*/

#include "Voronoi.h"

#define foreach(it, v) for(auto it=(v).begin(); it != (v).end(); ++it)

Voronoi::Voronoi(){
    edges = NULL;
}

Edges * Voronoi::GetEdges(Vertices * v, int w, int h){
    places = v, width = w, height = h, root = NULL;

    if(!edges)
        edges = new Edges();
    else{
        // clear all the data from the previous time the method was called
        foreach(it, points) delete(*it);
        foreach(it, *edges) delete(*it);
        points.clear();
        edges->clear();
    }
    
    // insert all point events into the event queue
    foreach(it, *places)
        queue.push(new VEvent(*it, true));

    // the sweepline moves from top to bottom
    while(!queue.empty()){
        VEvent * e = queue.top(); queue.pop();
        
        sweepline = e->point->y;
        
        if(deleted.find(e) != deleted.end()){
            delete(e);
            deleted.erase(e); 
            continue;
        }
        if(e->pe)
            InsertParabola(e->point);
        else
            RemoveParabola(e);
        
        delete(e);
    }
    
    FinishEdge(root);

    // complete all half edges that we need to build the Voronoi diagram
    foreach(it, *edges){
        if((*it)->twin){
            (*it)->start = (*it)->twin->end;
            delete (*it)->twin;
        }
    }
    
    return edges;
}

void Voronoi::InsertParabola(VPoint * p){
    if(!root){
        root = new VParabola(p);
        return;
    }

    // corner case, the two points are on the same line
    if(root->isLeaf and root->site->y - p->y < 1){
        VPoint * fp = root->site;
        root->isLeaf = false;
        root->SetLeft(new VParabola(fp));
        root->SetRight(new VParabola(p));
        
        // create a new Voronoi vertex
        VPoint * s = new VPoint((p->x + fp->x)/2, height);
        points.push_back(s);
        
        // create the edge at the current root node that separates the two points
        if(p->x > fp->x) root->edge = new VEdge(s, fp, p);
        else root->edge = new VEdge(s, p, fp);
        
        edges->push_back(root->edge);
        return;
    }

    // get the parabola that is just above this point
    VParabola * par = GetParabolaByX(p->x);
    
    // if there was a circle event with respect to this parabola,
    // then delete that circle event since this parabola will now be split
    // and thus any events with respect to this parabola will now be invalid
    if(par->cEvent){
        deleted.insert(par->cEvent);
        par->cEvent = NULL;
    }
    
    // GetY -> Get y coordinate of the point that is equidistant from the site that determines the parabola par
    // and the point p. This point is a new Voronoi vertex
    VPoint * start = new VPoint(p->x, GetY(par->site, p->x));
    points.push_back(start);

    // el and er are the two twin edges of the Voronoi edge that separates the two points
    VEdge * el = new VEdge(start, par->site, p);
    VEdge * er = new VEdge(start, p, par->site);

    el->twin = er;
    edges->push_back(el);

    // split the current parabola as follows
    
    //   par     =====>    par (er)
    //                     /     \
    //                   d (el)  pright
    //                   / \
    //                 pleft  pnow
    
    // par changes from a leave node to an internal node    
    
    par->edge = er;
    par->isLeaf = false;

    VParabola * pleft = new VParabola(par->site);
    VParabola * pnow = new VParabola(p);
    VParabola * pright = new VParabola(par->site);

    par->SetRight(pright);
    par->SetLeft(new VParabola());
    par->Left()->edge = el;

    par->Left()->SetLeft(pleft);
    par->Left()->SetRight(pnow);
    
    // check for circle events with pleft and pright as the middle points of the triplets
    // pleft p1 pright cannot create a circle event since they are determined by only 2 points
    CheckCircle(pleft);
    CheckCircle(pright);
}

void Voronoi::RemoveParabola(VEvent * e){
    VParabola * pcur = e->arch;
    
    VParabola * xl = VParabola::GetLeftSeparator(pcur);
    VParabola * xr = VParabola::GetRightSeparator(pcur);
    
    // get the parabolas that come just before and just after in the beach line
    VParabola * pleft = VParabola::GetLeftChild(xl);
    VParabola * pright = VParabola::GetRightChild(xr);

    // if the two parabolas are determined by the same vertex then we have an error
    assert(pleft != pright);

    // we must now delete any circle corresponding to pleft and pright since after pcur is deleted
    // they will be invalid
    if(pleft->cEvent){
        deleted.insert(pleft->cEvent);
        pleft->cEvent = NULL;
    }
    if(pright->cEvent){
        deleted.insert(pright->cEvent);
        pright->cEvent = NULL;
    }

    VPoint * p = new VPoint(e->point->x, GetY(pcur->site, e->point->x));
    points.push_back(p);

    xl->edge->end = p;
    xr->edge->end = p;
    
    // find the higher of the two parents xl and xr
    // we must update the Voronoi edge corresponding to the higher parent/
    // so as to correspond to the edge separating pleft and pright
    VParabola * higher;
    VParabola * tmp = pcur;
    while(tmp != root){
        tmp = tmp->parent;
        if(tmp == xl) higher = xl;
        if(tmp == xr) higher = xr;
    }
    higher->edge = new VEdge(p, pleft->site, pright->site);
    edges->push_back(higher->edge);
    
    // to delete the parabola, we replace its parent with its sibling
    // this gives rise the the following four cases
    VParabola * gparent = pcur->parent->parent;
    if(pcur->parent->Left() == pcur){
        if(gparent->Left() == pcur->parent) gparent->SetLeft (pcur->parent->Right());
        if(gparent->Right() == pcur->parent) gparent->SetRight(pcur->parent->Right());
    }
    else{
        if(gparent->Left() == pcur->parent) gparent->SetLeft (pcur->parent->Left());
        if(gparent->Right() == pcur->parent) gparent->SetRight(pcur->parent->Left());
    }

    // delete the node and its parent from the binary tree
    delete pcur->parent;
    delete pcur;

    // there may exist a new circle event corresponding to pleft and pright since their twins have now changed
    CheckCircle(pleft);
    CheckCircle(pright);
}

// if there are no more events left to process, the parabolas left in the search tree
// correspond to those sites with unbounded faces. We must finish these unbounded edges with respect
// to the bounding box for our Voronoi diagram
void Voronoi::FinishEdge(VParabola * n){
    if(n->isLeaf){ 
    	return void(delete n);
    }
    
    double mx;
    if(n->edge->direction->x > 0.0)
        mx = width;
    else if(n->edge->direction->x < 0.0)
        mx = 0;
    else{
        // corner case
        // the edge is a vertical line and hence has slope infinity
        
        VPoint * end = new VPoint(n->edge->start->x, 0); 
        n->edge->end = end;
        points.push_back(end);
            
        FinishEdge(n->Left() );
        FinishEdge(n->Right());
        
        delete n;
        
        return;
    }
    
    // substitute mx into the equation of the line for the edge
    // y = m x + c to obtain the y-coordinate for the end point of this edge
    VPoint * end = new VPoint(mx, mx * n->edge->m + n->edge->c); 
    n->edge->end = end;
    points.push_back(end);
    
    // recursively finish the edges in the left and right subtrees
    FinishEdge(n->Left());
    FinishEdge(n->Right());
    
    // we are done! 
    // delete the current node n
    delete n;
    return;
}


double    Voronoi::GetXOfEdge(VParabola * par, double y){
    VParabola * left = VParabola::GetLeftChild(par);        // O(log n)
    VParabola * right= VParabola::GetRightChild(par);        // O(log n)

    // y = a1 x^2 + b1 x + c1
    // y = a2 x^2 + b2 x + c2
    // solution for points of intersection are given by
    // (a1 - a2) x^2 + (b1 - b2) y + (c1 - c2) = 0
    // since the parabolas always intersect, the discriminant of the above quadratic is always non-negative

    VPoint * p = left->site;
    VPoint * r = right->site;

    double fac;
    
    // a1, b1, c1 and a2, b2, c2 have the same meaning as in the comment above
    fac = 2 * (p->y - y);
    double a1 = 1.0 / fac;
    double b1 = -2 * p->x / fac;
    double c1 = y + fac / 4 + p->x * p->x / fac;
            
    fac = 2 * (r->y - y);
    double a2 = 1 / fac;
    double b2 = -2 * r->x/fac;
    double c2 = sweepline + fac / 4 + r->x * r->x / fac;
            
    double a = a1 - a2;
    double b = b1 - b2;
    double c = c1 - c2;
    
    double sqrtD = sqrt(b * b - 4 * a * c);
    double x1 = (-b + sqrtD) / (2 * a);
    double x2 = (-b - sqrtD) / (2 * a);

    // two parabolas can have two points of intersection and hence we may have two distinct roots

    // the new parabola always sits in the middle
    // if the point responsible for the new parabola is on the left, then we should consider the larger x-coordinate
    // if the point responsible for the new parabola is on the right, then we should consider the smaller x-coordinate
    //
    //     new parabola
    //     v 
    // p0 p1 p0
    //

    if(p->y < r->y ) return max(x1, x2);
    else return min(x1, x2);
}

// traverse through the binary search tree to find the parabola that lies above point at xx
VParabola * Voronoi::GetParabolaByX(double xx){
    VParabola * par = root;

    while(!par->isLeaf){
        // get current x co-ordinate of this edge with respect to the current position of the sweepline
        double x = GetXOfEdge(par, sweepline);
        if(x > xx)
            par = par->Left();
        else
            par = par->Right();                
    }
    
    return par;
}

// get the y-coordinate of the point on the parabola p with x-coordinate as x when the sweepline is at position sweepline
// the parabola is represented by the equation y = a*x^2 + b*x + c
double    Voronoi::GetY(VPoint * p, double x){
    double fac = 2 * (p->y - sweepline);
    double a = 1 / fac;
    double b = -2 * p->x / fac;
    double c = sweepline + fac / 4 + p->x * p->x / fac;
    
    return a*x*x + b*x + c;
}

// check for the existence of a circle event with point b as the center
void Voronoi::CheckCircle(VParabola * b){
    VParabola * lp = VParabola::GetLeftSeparator(b);
    VParabola * rp = VParabola::GetRightSeparator(b);

    VParabola * a  = VParabola::GetLeftChild(lp);
    VParabola * c  = VParabola::GetRightChild(rp);
    
    // the three parabolas that can cause a circle event 
    // occur in the beach line in the order a, b, c
    
    // if either of the left or right twins of b don't exist or they are determined by the same site
    // then we cannot have a circle event
    if(!a or !c or a->site == c->site) return;

    // s is the center of the circle that passes through the sites of parabolas
    // a, b, and c
    VPoint * s = 0;
    s = GetEdgeIntersection(lp->edge, rp->edge);
    
    // if the edges don't interesect, then there cannot be a circle events
    // such a case can arise when the edges are unbounded edges
    if(!s) return;
    
    double dx = a->site->x - s->x;
    double dy = a->site->y - s->y;

    // r is the radius of the circle that passes through a, b and c
    double r = sqrt((dx * dx) + (dy * dy));
    
    // the circumcircle should be at least partially below the sweepline in the general assumption
    if(s->y - r >= sweepline) return;
    
    // the new event point will be when the sweepline is at the bottom most point of the circle
    VEvent * e = new VEvent(new VPoint(s->x, s->y - r), false);
    
    points.push_back(e->point);
    
    b->cEvent = e;
    e->arch = b;
    
    queue.push(e);
}

VPoint * Voronoi::GetEdgeIntersection(VEdge * a, VEdge * b){
    double x = (b->c - a->c) / (a->m - b->m);
    double y = a->m * x + a->c;
    
    // handle the cases when the edges don't intersect
    if((x - a->start->x)/a->direction->x < 0) return NULL;
    if((y - a->start->y)/a->direction->y < 0) return NULL;
    if((x - b->start->x)/b->direction->x < 0) return NULL;
    if((y - b->start->y)/b->direction->y < 0) return NULL;    

    // p is the point of intersection of edges a and b
    // This is a new Voronoi vertex
    VPoint * p = new VPoint(x, y);        
    points.push_back(p);
    
    return p;
}

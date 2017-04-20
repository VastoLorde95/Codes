/*
    Implementation of the VParabola class. For class description refer VParabola.h
*/

#include "VParabola.h"
#include "VPoint.h"

// Edge constructor
VParabola::VParabola(){
    site = NULL;
    isLeaf = false;
    cEvent = NULL;
    edge = NULL;
    parent = NULL;
}

// Leaf constructor
VParabola::VParabola(VPoint * s){
    site = s; 
    isLeaf = true;
    cEvent = NULL;
    edge = NULL;
    parent = NULL;
}

void VParabola::SetLeft (VParabola * p){
    _left  = p;
    p->parent = this;
}

void VParabola::SetRight(VParabola * p){
    _right = p;
    p->parent = this;
}

VParabola *    VParabola::Left (){
    return _left;
}

VParabola * VParabola::Right(){
    return _right;
}

// while you are a left child, ascend the tree
// stop once you are a right child and report the answer
// opposite of get right child
VParabola * VParabola::GetLeftSeparator(VParabola * p){
    VParabola * par    = p->parent;
    VParabola * pLast = p;
    while(par->Left() == pLast){ 
        if(!par->parent)
            return NULL;
        else{
            pLast = par; 
            par = par->parent; 
        }
    }
    return par;
}


// while you are a right child, ascend the tree
// stop once you are a left child and report the answer
// opposite of get left child
VParabola * VParabola::GetRightSeparator(VParabola * p){
    VParabola * par = p->parent;
    VParabola * pLast = p;
    while(par->Right() == pLast){ 
        if(!par->parent)
            return NULL;
        else{
            pLast = par; par = par->parent; 
        }
    }
    return par;
}

VParabola * VParabola::GetLeftChild(VParabola * p){
    if(!p) return NULL;
    VParabola * par = p->Left();
    while(!par->isLeaf) par = par->Right();
    return par;
}

VParabola * VParabola::GetRightChild(VParabola * p){
    if(!p) return NULL;
    VParabola * par = p->Right();
    while(!par->isLeaf) par = par->Left();
    return par;
}

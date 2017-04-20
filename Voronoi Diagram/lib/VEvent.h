/*
    Class Description

    This class stores the line sweep events. Events are of two types - points events and circle events

    public attributes:
    point  : the point at which current event occurs (bottom of the circle for circle events and focus point for point event)
    pe     : a boolean which indicates whether it is a point event or not
    y      : y coordinate of the event, events are sorted by this attribute, the largest event being processed first
    arch   : if this is a point event, arch stores a pointer to the parabola just above the focus
    
    public methods:
    VEvent : constructor
    
    it takes the following arguments as input
    p    : point at which the event occurs
    pev  : if true, then this event is a point event and a circle event othewise
    
    custom compartor to establish a total ordering among events so that we can implement line sweep
*/

#ifndef VEvent_h
#define VEvent_h

#include "VPoint.h"
#include "VParabola.h"

class VEvent{
public:
    VPoint *point;
    bool pe;
    double y;
    VParabola *arch;

    VEvent(VPoint * p, bool pev){
        point = p;
        pe = pev;
        y = p->y;
        arch = 0;
    }

    struct CompareEvent : public std::binary_function<VEvent*, VEvent*, bool>{
        bool operator()(const VEvent* l, const VEvent* r) const {
            return (l->y < r->y);
        }
    };
};

#endif

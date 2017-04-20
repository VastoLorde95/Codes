/*
    Container for a point
*/

#ifndef VPoint_h
#define VPoint_h

struct VPoint{
    const double x, y;
    VPoint(double nx, double ny) : x(nx), y(ny) { }
};

#endif

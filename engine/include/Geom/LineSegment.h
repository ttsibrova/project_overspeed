#pragma once
#include <Geom/Point2D.h>

namespace geom
{

struct LineSegment
{
    Point2D first;
    Point2D last;

    bool contains (const Point2D& pnt, const float tolerance);
};

}

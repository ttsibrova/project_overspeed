#pragma once
#include <Geom/Point2D.h>
#include <Geom/Precision.h>

namespace geom
{

struct LineSegment
{
    Point2D first;
    Point2D last;

    bool contains (const Point2D& pnt, const float tolerance = precision::halfPixel) const
    {
        return pnt.x >= std::min (first.x, last.x) - tolerance && pnt.x <= std::max (first.x, last.x) + tolerance &&
               pnt.y >= std::min (first.y, last.y) - tolerance && pnt.y <= std::max (first.y, last.y) + tolerance;
    }
};

}

#pragma once

#include <Physics/Point2D.h>
#include <optional>

namespace geom
{

[[nodiscard]] std::optional <phs::Point2D> IntersectLines (const phs::Point2D& start1,
                                                           const phs::Point2D& end1,
                                                           const phs::Point2D& start2,
                                                           const phs::Point2D& end2)
{
    float x1, x2, x3, x4, y1, y2, y3, y4;
    x1 = start1.X();
    x2 = end1.X();
    x3 = start2.X();
    x4 = end2.X();
    y1 = start1.Y();
    y2 = end1.Y();
    y3 = start2.Y();
    y4 = end2.Y();

    float coef = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (std::abs (coef) < 1e-4)
        return std::nullopt;

    float x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / coef;
    float y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / coef;
    
    return phs::Point2D (x, y);
}

}


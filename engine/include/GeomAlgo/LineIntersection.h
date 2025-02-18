#pragma once

#include <Geom/Point2D.h>
#include <optional>

namespace geom::algo {

[[nodiscard]] std::optional <Point2D> IntersectLines (const Point2D& start1,
                                                      const Point2D& end1,
                                                      const Point2D& start2,
                                                      const Point2D& end2)
{

    if (start1 == start2 || start1 == end2) {
        return start1;
    }

    if (end1 == start2 || end1 == end2) {
        return end1;
    }

    const float x1 = start1.X();
    const float x2 = end1.X();
    const float x3 = start2.X();
    const float x4 = end2.X();
    const float y1 = start1.Y();
    const float y2 = end1.Y();
    const float y3 = start2.Y();
    const float y4 = end2.Y();

    const float A1 = y1 - y2;
    const float B1 = x2 - x1;
    const float C1 = y1 * x2 - x1 * y2;

    const float A2 = y3 - y4;
    const float B2 = x4 - x3;
    const float C2 = y3 * x4 - x3 * y4;

    const float D = A1 * B2 - B1 * A2;
    if (std::abs (D) < precision::float_tol)
        return std::nullopt;

    const float x = (C1 * B2 - B1 * C2) / D;
    const float y = (A1 * C2 - C1 * A2) / D;
    return Point2D (x, y);
}

}


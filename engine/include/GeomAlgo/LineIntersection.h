#pragma once

#include <Geom/LineSegment.h>
#include <optional>

namespace geom::algo {

[[nodiscard]] std::optional<Point2D> intersectLines (const Point2D& start1, const Point2D& end1,
                                                     const Point2D& start2, const Point2D& end2)
{

    if (start1 == start2 || start1 == end2) {
        return start1;
    }

    if (end1 == start2 || end1 == end2) {
        return end1;
    }

    const float x1 = start1.x;
    const float x2 = end1.x;
    const float x3 = start2.x;
    const float x4 = end2.x;
    const float y1 = start1.y;
    const float y2 = end1.y;
    const float y3 = start2.y;
    const float y4 = end2.y;

    const float A1 = y1 - y2;
    const float B1 = x2 - x1;
    const float C1 = y1 * x2 - x1 * y2;

    const float A2 = y3 - y4;
    const float B2 = x4 - x3;
    const float C2 = y3 * x4 - x3 * y4;

    const float D = A1 * B2 - B1 * A2;
    if (std::abs (D) < precision::floatTol) {
        return std::nullopt;
    }

    const float x = (C1 * B2 - B1 * C2) / D;
    const float y = (A1 * C2 - C1 * A2) / D;
    return Point2D (x, y);
}

[[nodiscard]] std::optional<Point2D> intersectSegments (const LineSegment& segment1, const LineSegment& segment2)
{
    const auto res = intersectLines(segment1.first, segment1.last, segment2.first, segment2.last);
    if (res.has_value() && segment1.contains(res.value()) && segment2.contains(res.value())) {
        return res.value();
    }
    return std::nullopt;
}

} // namespace geom::algo

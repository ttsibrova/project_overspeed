#include <World/LevelCollision.h>

#include <Geometry/LineIntersection.h>
#include <Physics/Precisions.h>

#include <cassert>
#include <print>

#include <Debug/DebugLog.h>

namespace Collision
{
namespace {


std::vector <std::pair <phs::Point2D, phs::Point2D>> GetTileLines (phs::Quadrant quadrant,
                                                                   const phs::Point2D& minTileCorner,
                                                                   const phs::Point2D& maxTileCorner)
{
    std::vector <std::pair <phs::Point2D, phs::Point2D>> res;
    switch (quadrant)
    {
    case phs::Quadrant::I:
        res.emplace_back (phs::Point2D (minTileCorner.X(), maxTileCorner.Y()), maxTileCorner);
        res.emplace_back (phs::Point2D (maxTileCorner.X(), minTileCorner.Y()), maxTileCorner);
        break;
    case phs::Quadrant::II:
        res.emplace_back (phs::Point2D (minTileCorner.X(), maxTileCorner.Y()), maxTileCorner);
        res.emplace_back (minTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case phs::Quadrant::III:
        res.emplace_back (minTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        res.emplace_back (minTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    case phs::Quadrant::IV:
        res.emplace_back (minTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        res.emplace_back (phs::Point2D (maxTileCorner.X(), minTileCorner.Y()), maxTileCorner);
        break;
    case phs::Quadrant::X_ALIGNED:
        res.emplace_back (maxTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    case phs::Quadrant::Y_ALIGNED:
        res.emplace_back (maxTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case phs::Quadrant::X_OPPOSITE:
        res.emplace_back (minTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case phs::Quadrant::Y_OPPOSITE:
        res.emplace_back (minTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    }
    return res;
}

bool IsBelongToSegment (const phs::Point2D& p, const phs::Point2D& p1, const phs::Point2D& p2)
{
    return p.X() >= std::min (p1.X(), p2.X()) - phs::Precision::half_pixel && p.X() <= std::max (p1.X(), p2.X()) + phs::Precision::half_pixel &&
           p.Y() >= std::min (p1.Y(), p2.Y()) - phs::Precision::half_pixel && p.Y() <= std::max (p1.Y(), p2.Y()) + phs::Precision::half_pixel;

}


std::optional <phs::Vector2D> SweepCollision (const phs::Collider& playerCollider,
                                              const phs::Vector2D& playerTrsl,
                                              phs::Point2D tileMinCorner,
                                              phs::Point2D tileMaxCorner)
{
    float colliderHalfHeight = playerCollider.Height() / 2.f;
    float colliderHalfWidth = playerCollider.Width() / 2.f;
    phs::Point2D colliderKeyPnt (playerCollider.Min().X() + colliderHalfWidth, playerCollider.Min().Y() + colliderHalfHeight);
    phs::Point2D trajectoryEndPnt = colliderKeyPnt.Translated (playerTrsl);

    //std::print ("Collider min pnt - X: {}, Y: {}\n", playerCollider.Min().X(), playerCollider.Min().Y());
    //std::print ("Collider max pnt - X: {}, Y: {}\n", playerCollider.Max().X(), playerCollider.Max().Y());

    //std::print ("Collider key pnt - X: {}, Y: {}\n", colliderKeyPnt.X(), colliderKeyPnt.Y());
    //std::print ("Trajectory end pnt - X: {}, Y: {}\n", trajectoryEndPnt.X(), trajectoryEndPnt.Y());

    //std::print ("Tile min pnt - X: {}, Y: {}\n", tileMinCorner.X(), tileMinCorner.Y());
    //std::print ("Tile max pnt - X: {}, Y: {}\n", tileMaxCorner.X(), tileMaxCorner.Y());

    tileMinCorner.X() -= colliderHalfWidth;
    tileMinCorner.Y() -= colliderHalfHeight;

    tileMaxCorner.X() += colliderHalfWidth;
    tileMaxCorner.Y() += colliderHalfHeight;

    //std::print ("Updated tile min pnt - X: {}, Y: {}\n", tileMinCorner.X(), tileMinCorner.Y());
    //std::print ("Updated tile max pnt - X: {}, Y: {}\n", tileMaxCorner.X(), tileMaxCorner.Y());



    auto tileLines = GetTileLines (phs::GetInversedQuadrant (phs::GetVectorQudrant (playerTrsl)), tileMinCorner, tileMaxCorner);

    std::optional <phs::Vector2D> newTrsl;
    for (const auto line: tileLines) {
        auto intersectPnt = geom::IntersectLines (colliderKeyPnt, trajectoryEndPnt, line.first, line.second);
        if (!intersectPnt.has_value()) {
            std::print ("Intersector failed to find value, manual branch DISABLED\n");
            ////Embracing a specific issue where our vector from chosen point strinctly aligned with
            ////tile side. Selecting the point that is basically closes to original character position
            //phs::Quadrant vQ = phs::GetVectorQudrant (playerTrsl);
            //switch (vQ)
            //{
            //case phs::Quadrant::I:
            //case phs::Quadrant::II:
            //case phs::Quadrant::III:
            //case phs::Quadrant::IV:
            //    //TODO update this after adding slopes
            //    break;
            //case phs::Quadrant::X_ALIGNED:
            //{
            //    float minX = std::min ({trajectoryEndPnt.X(), line.first.X(), line.second.X()});
            //    assert (trajectoryEndPnt.Y() == colliderKeyPnt.Y() && line.first.Y() == line.second.Y());
            //    intersectPnt = phs::Point2D (minX, trajectoryEndPnt.Y());
            //    break;
            //}
            //case phs::Quadrant::Y_ALIGNED:
            //{
            //    float minY = std::min ({trajectoryEndPnt.Y(), line.first.Y(), line.second.Y()});
            //    assert (trajectoryEndPnt.X() == colliderKeyPnt.X() && line.first.X() == line.second.X());
            //    intersectPnt = phs::Point2D (trajectoryEndPnt.X(), minY);
            //    break;
            //}
            //case phs::Quadrant::X_OPPOSITE:
            //{
            //    float maxX = std::max ({trajectoryEndPnt.X(), line.first.X(), line.second.X()});
            //    assert (trajectoryEndPnt.Y() == colliderKeyPnt.Y() && line.first.Y() == line.second.Y());
            //    intersectPnt = phs::Point2D (maxX, trajectoryEndPnt.Y());
            //    break;
            //}
            //case phs::Quadrant::Y_OPPOSITE:
            //{
            //    float maxY = std::max ({trajectoryEndPnt.Y(), line.first.Y(), line.second.Y()});
            //    assert (trajectoryEndPnt.X() == colliderKeyPnt.X() && line.first.X() == line.second.X());
            //    intersectPnt = phs::Point2D (trajectoryEndPnt.X(), maxY);
            //    break;
            //}
            //default:
            //    break;
            //}
        } 
        else if (!IsBelongToSegment (intersectPnt.value(), colliderKeyPnt, trajectoryEndPnt) ||
                 !IsBelongToSegment (intersectPnt.value(), line.first, line.second))
        {
            //std::print ("Intersector found the value X:{}, Y:{}, but it was rejected\n", intersectPnt.value().X(), intersectPnt.value().Y());
            continue;
        }
        Debug::Log (line, Debug::Collision);
        if (intersectPnt.has_value()) {
            //std::print ("Current line - X: {}, Y: {} -- X: {}, Y: {}\n", line.first.X(), line.first.Y(), line.second.X(), line.second.Y());
            //std::print ("Intersect pnt - X: {}, Y: {}\n", intersectPnt.value().X(), intersectPnt.value().Y());
        }
        if (!newTrsl.has_value() && intersectPnt.has_value()) {
            newTrsl = phs::Vector2D (intersectPnt.value().X() - colliderKeyPnt.X(), intersectPnt.value().Y() - colliderKeyPnt.Y());
        }
        if (newTrsl.has_value() && intersectPnt.has_value()) {
            auto candidate = phs::Vector2D (intersectPnt.value().X() - colliderKeyPnt.X(), intersectPnt.value().Y() - colliderKeyPnt.Y());
            if (candidate.SquareMagnitude() < newTrsl.value().SquareMagnitude()) {
                newTrsl = candidate;
            }
        }
    }

    if (newTrsl.has_value()) {
        auto dir = playerTrsl.Normalized();
        float newLength = newTrsl.value().Magnitude();
        //std::print ("Computed adj vector - X: {}, Y: {}\n", newTrsl.value().X(), newTrsl.value().Y());
        if (newLength < phs::Precision::float_tol) {
            newTrsl = phs::Vector2D();
        } else {
            newTrsl = dir * newLength;
        }
        //std::print ("Adjusted adj vector - X: {}, Y: {}\n", newTrsl.value().X(), newTrsl.value().Y());
    }
    return newTrsl;
}

}

std::optional <phs::Vector2D> HitScanGround (const phs::Collider& playerCollider, const phs::Vector2D& playerTrsl, const GroundData& groundData)
{
    phs::Collider playerNewCollider = playerCollider.Translated (playerTrsl);

    auto minPnt = playerNewCollider.Min();
    auto maxPnt = playerNewCollider.Max();

    size_t left_tile = static_cast <size_t> (std::floorf ((minPnt.X() + phs::Precision::quarter_pixel) / groundData.tile_width));
    if (left_tile > groundData.tiles.extent (0) - 1) {
        left_tile = 0U;
    }
    size_t right_tile = static_cast <size_t> (std::floorf ((maxPnt.X() - phs::Precision::quarter_pixel) / groundData.tile_width));
    right_tile = std::min (right_tile, groundData.tiles.extent (0) - 1);
    size_t top_tile = static_cast <size_t> (std::floorf ((minPnt.Y() + phs::Precision::quarter_pixel) / groundData.tile_height));
    if (top_tile > groundData.tiles.extent (1) - 1) {
        top_tile = 0U;
    }
    size_t bottom_tile = static_cast <size_t> (std::floorf ((maxPnt.Y() - phs::Precision::quarter_pixel) / groundData.tile_height));
    bottom_tile = std::min (bottom_tile, groundData.tiles.extent (1) - 1);

    std::optional <phs::Vector2D> finalVector;
    for (size_t i = left_tile; i <= right_tile; i++) {
        for (size_t j = top_tile; j <= bottom_tile; j++) {
            if (groundData.tiles[std::array{i, j}] != 0) {
                phs::Point2D tileMinCorner (static_cast <float> (i) * groundData.tile_width, static_cast <float> (j) * groundData.tile_height);
                phs::Point2D tileMaxCorner (tileMinCorner.X() + groundData.tile_width, tileMinCorner.Y() + groundData.tile_height);
                Debug::Log (std::make_pair (tileMinCorner, tileMaxCorner), Debug::Collision);
                auto adjustedVec = SweepCollision (playerCollider, playerTrsl, tileMinCorner, tileMaxCorner);
                if (!finalVector.has_value() ||
                    finalVector.has_value() && adjustedVec.has_value() && finalVector.value().SquareMagnitude() > adjustedVec.value().SquareMagnitude()) {
                    finalVector = adjustedVec;
                }
            }
        }
    }

    return finalVector;
}

std::optional<phs::Vector2D> GetGroundNormalUnderPlayer (const phs::Collider& playerCollider,
                                                         phs::Quadrant playerVelocityQuadrant,
                                                         const GroundData& groundData)
{
    phs::Point2D anchorPnt;
    switch (playerVelocityQuadrant)
    {
    case phs::Quadrant::I:
    case phs::Quadrant::IV:
    case phs::Quadrant::X_ALIGNED:
    {
        anchorPnt.X() = playerCollider.Max().X() + phs::Precision::pixel + phs::Precision::quarter_pixel;
        anchorPnt.Y() = playerCollider.Max().Y() + phs::Precision::pixel;
        break;
    }
    case phs::Quadrant::II:
    case phs::Quadrant::III:
    case phs::Quadrant::X_OPPOSITE:
    {
        anchorPnt.X() = playerCollider.Min().X() - phs::Precision::pixel + phs::Precision::quarter_pixel;
        anchorPnt.Y() = playerCollider.Max().Y() + phs::Precision::pixel;
        break;
    }
    case phs::Quadrant::Y_ALIGNED:
    case phs::Quadrant::Y_OPPOSITE:
    {
        assert (false); // Is this really possible case?
        return std::nullopt;
    }
    }

    size_t tile_x = static_cast <size_t> (std::floorf (anchorPnt.X() / groundData.tile_width));
    size_t tile_y = static_cast <size_t> (std::floorf (anchorPnt.Y() / groundData.tile_height));

    tile_x = std::min (tile_x, groundData.tiles.extent(0) - 1);
    tile_y = std::min (tile_y, groundData.tiles.extent(1) - 1);

    if (tile_y == 0) {
        return std::nullopt;
    }

    if (groundData.tiles[std::array {tile_x, tile_y}] == 0 || // block is not a ground
        groundData.tiles[std::array {tile_x, tile_y - 1}] != 0) //there is a block on top, not a ground case
    {
        return std::nullopt;
    }

    //TODO return vector should depend on block type
    return phs::GetUpVector();
}

bool IsPlayerGrounded (const phs::Collider& playerCollider, const GroundData& groundData)
{
    auto minPnt = playerCollider.Min();
    auto maxPnt = playerCollider.Max();

    float center = minPnt.X() + playerCollider.Width() / 2;
    size_t tile_center = static_cast <size_t> (std::floorf (center / groundData.tile_width));
    size_t tile_right = static_cast <size_t> (std::floorf ((maxPnt.X() - phs::Precision::quarter_pixel) / groundData.tile_width));
    size_t tile_left = static_cast <size_t> (std::floorf ((minPnt.X() + phs::Precision::quarter_pixel) / groundData.tile_width));
    size_t tile_y = static_cast <size_t> (std::ceilf (maxPnt.Y() / groundData.tile_height));
    tile_y = std::min (groundData.tiles.extent(1) - 1, tile_y);
    tile_center = std::min (groundData.tiles.extent(0) - 1, tile_center);
    tile_right = std::min (groundData.tiles.extent(0) - 1, tile_right);
    tile_left = std::min (groundData.tiles.extent(0) - 1, tile_left);

    if (groundData.tiles[std::array{tile_left, tile_y}] == 0 &&
        groundData.tiles[std::array{tile_center, tile_y}] == 0 && 
        groundData.tiles[std::array{tile_right, tile_y}] == 0) {
        return false;
    }

    std::print ("Distance to ground: {}\n", static_cast <float> (tile_y) * groundData.tile_height - maxPnt.Y());

    return static_cast <float> (tile_y) * groundData.tile_height - maxPnt.Y() < phs::Precision::quarter_pixel;
}

}

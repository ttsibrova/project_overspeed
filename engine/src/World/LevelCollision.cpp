#include <World/LevelCollision.h>

#include <Geometry/LineIntersection.h>

#include <cassert>
#include <print>

#include <Debug/DebugLog.h>

namespace Collision
{
namespace {
std::optional <phs::Point2D> GetCollidedPoint (const Collider& playerCollider,
                                               phs::Quadrant quadrant,
                                               const phs::Point2D& minTileCorner,
                                               const phs::Point2D& maxTileCorner)
{
    auto min = playerCollider.Min();
    auto max = playerCollider.Max();
    float center_X = min.X() + playerCollider.Width() / 2;
    float center_Y = min.Y() + playerCollider.Height() / 2;

    std::vector <phs::Point2D> res;
    switch (quadrant)
    {
    case phs::Quadrant::I:
        res.push_back (max);
        res.emplace_back (min.X(), max.Y());
        res.emplace_back (max.X(), min.Y());
        res.emplace_back (max.X(), center_Y);
        res.emplace_back (center_X, max.Y());
        break;
    case phs::Quadrant::II:
        res.emplace_back (min.X(), max.Y());
        res.push_back (min);
        res.push_back (max);
        res.emplace_back (center_X, max.Y());
        res.emplace_back (min.X(), center_Y);
        break;
    case phs::Quadrant::III:
        res.push_back (min);
        res.emplace_back (min.X(), max.Y());
        res.emplace_back (min.X(), max.Y());
        res.emplace_back (min.X(), center_Y);
        res.emplace_back (center_X, min.Y());
        break;
    case phs::Quadrant::IV:
        res.emplace_back (max.X(), min.Y());
        res.push_back (min);
        res.push_back (max);
        res.emplace_back (center_X, min.Y());
        res.emplace_back (max.X(), center_Y);
        break;
    case phs::Quadrant::X_ALIGNED:
        res.emplace_back (max.X(), center_Y);
        res.emplace_back (max.X(), min.Y());
        res.emplace_back (max.X(), max.Y() - 1.e-4f);
        break;
    case phs::Quadrant::Y_ALIGNED:
        res.emplace_back (center_X, max.Y());
        res.push_back (max);
        res.emplace_back (min.X(), max.Y());
        break;
    case phs::Quadrant::X_OPPOSITE:
        res.emplace_back (min.X(), center_Y);
        res.push_back (min);
        res.emplace_back (min.X(), max.Y() - 1.e-4f);
        break;
    case phs::Quadrant::Y_OPPOSITE:
        res.emplace_back (center_X, min.Y());
        res.push_back (min);
        res.emplace_back (max.X(), min.Y());
        break;
    }

    for (const auto& pnt : res) {
        if (pnt.X() >= minTileCorner.X() && pnt.X() <= maxTileCorner.X() && pnt.Y() >= minTileCorner.Y() && pnt.Y() <= maxTileCorner.Y()) {
            return pnt;
        }
    }
    return std::nullopt;
}

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
        res.emplace_back (minTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        res.emplace_back (phs::Point2D (minTileCorner.X(), maxTileCorner.Y()), maxTileCorner);
        break;
    case phs::Quadrant::Y_ALIGNED:
        res.emplace_back (maxTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        res.emplace_back (maxTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        res.emplace_back (minTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case phs::Quadrant::X_OPPOSITE:
        res.emplace_back (minTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        res.emplace_back (phs::Point2D (minTileCorner.X(), maxTileCorner.Y()), maxTileCorner);
        res.emplace_back (minTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    case phs::Quadrant::Y_OPPOSITE:
        res.emplace_back (minTileCorner, phs::Point2D (maxTileCorner.X(), minTileCorner.Y()));
        res.emplace_back (minTileCorner, phs::Point2D (minTileCorner.X(), maxTileCorner.Y()));
        res.emplace_back (maxTileCorner, phs::Point2D (maxTileCorner.X (), minTileCorner.Y ()));
        break;
    }
    return res;
}

bool IsBelongToSegment (const phs::Point2D& p, const phs::Point2D& p1, const phs::Point2D& p2)
{
    return p.X() >= std::min (p1.X(), p2.X()) - 0.5f && p.X() <= std::max (p1.X(), p2.X()) + 0.5f &&
           p.Y() >= std::min (p1.Y(), p2.Y()) - 0.5f && p.Y() <= std::max (p1.Y(), p2.Y()) + 0.5f;

}

phs::Vector2D ComputeAdjustment (const phs::Point2D& anchorPnt,
                                 const phs::Vector2D& fullVector,
                                 const std::vector <std::pair <phs::Point2D, phs::Point2D>>& tileLines)
{
    auto inversedVec = fullVector.Fipped();
    auto secondPnt = anchorPnt.Translated (inversedVec);

    for (const auto& line: tileLines) {
        auto intersectPnt = geom::IntersectLines (anchorPnt, secondPnt, line.first, line.second);
        if (!intersectPnt.has_value()) {
            std::print ("Intersector failed to find value, manual branch\n");
            //Embracing a specific issue where our vector from chosen point strinctly aligned with
            //tile side. Selecting the point that is basically closes to original character position
            phs::Quadrant vQ = phs::GetVectorQudrant (fullVector);
            switch (vQ)
            {
            case phs::Quadrant::I:
            case phs::Quadrant::II:
            case phs::Quadrant::III:
            case phs::Quadrant::IV:
                //TODO update this after adding slopes
                break;
            case phs::Quadrant::X_ALIGNED:
            {
                float minX = std::min ({anchorPnt.X(), line.first.X(), line.second.X()});
                assert (anchorPnt.Y() == secondPnt.Y() && line.first.Y() == line.second.Y());
                intersectPnt = phs::Point2D (minX, anchorPnt.Y());
                break;
            }
            case phs::Quadrant::Y_ALIGNED:
            {
                float minY = std::min ({anchorPnt.Y(), line.first.Y(), line.second.Y()});
                assert (anchorPnt.X() == secondPnt.X() && line.first.X() == line.second.X());
                intersectPnt = phs::Point2D (anchorPnt.X(), minY);
                break;
            }
            case phs::Quadrant::X_OPPOSITE:
            {
                float maxX = std::max ({anchorPnt.X(), line.first.X(), line.second.X()});
                assert (anchorPnt.Y() == secondPnt.Y() && line.first.Y() == line.second.Y());
                intersectPnt = phs::Point2D (maxX, anchorPnt.Y());
                break;
            }
            case phs::Quadrant::Y_OPPOSITE:
            {
                float maxY = std::max ({anchorPnt.Y(), line.first.Y(), line.second.Y()});
                assert (anchorPnt.X() == secondPnt.X() && line.first.X() == line.second.X());
                intersectPnt = phs::Point2D (anchorPnt.X(), maxY);
                break;
            }
            default:
                break;
            }

        } else if (!IsBelongToSegment (intersectPnt.value(), anchorPnt, secondPnt) ||
                   !IsBelongToSegment (intersectPnt.value(), line.first, line.second))
        {
            std::print ("Intersector found the value X:{}, Y:{}, but it was rejected\n", intersectPnt.value().X(), intersectPnt.value().Y());
            continue;
        }
        Debug::Log (line, Debug::Collision);
        return phs::Vector2D (intersectPnt.value().X() - secondPnt.X(), intersectPnt.value().Y() - secondPnt.Y());
    }
    assert (false);
    return phs::Vector2D();
}

}

std::optional <phs::Vector2D> HitScanGround (const Collider& playerCollider, const phs::Vector2D& playerTrsl, const GroundData& groundData)
{
    Collider playerNewCollider = playerCollider.Translated (playerTrsl);

    auto minPnt = playerNewCollider.Min();
    auto maxPnt = playerNewCollider.Max();

    size_t left_tile = static_cast <size_t> (std::floorf (minPnt.X() / groundData.tile_width));
    if (left_tile > groundData.tiles.extent (0) - 1) {
        left_tile = 0U;
    }
    size_t right_tile = static_cast <size_t> (std::floorf (maxPnt.X() / groundData.tile_width));
    right_tile = std::min (right_tile, groundData.tiles.extent (0) - 1);
    size_t top_tile = static_cast <size_t> (std::floorf (minPnt.Y() / groundData.tile_height));
    if (top_tile > groundData.tiles.extent (1) - 1) {
        top_tile = 0U;
    }
    size_t bottom_tile = static_cast <size_t> (std::floorf (maxPnt.Y() / groundData.tile_height));
    bottom_tile = std::min (bottom_tile, groundData.tiles.extent (1) - 1);

    auto vecQuadrant = phs::GetVectorQudrant (playerTrsl);
    std::optional <phs::Vector2D> finalVector;
    for (size_t i = left_tile; i <= right_tile; i++) {
        for (size_t j = top_tile; j <= bottom_tile; j++) {
            if (groundData.tiles[std::array{i, j}] != 0) {
                phs::Point2D tileMinCorner (static_cast <float> (i) * groundData.tile_width, static_cast <float> (j) * groundData.tile_height);
                phs::Point2D tileMaxCorner (tileMinCorner.X() + groundData.tile_width, tileMinCorner.Y() + groundData.tile_height);
                Debug::Log (std::make_pair (tileMinCorner, tileMaxCorner), Debug::Collision);
                auto collidedPnt = GetCollidedPoint (playerNewCollider, vecQuadrant, tileMinCorner, tileMaxCorner);
                if (!collidedPnt.has_value()) {
                    continue;
                }
                auto tileLines = GetTileLines (phs::GetInversedQuadrant (vecQuadrant), tileMinCorner, tileMaxCorner);
                auto adjustedVec = ComputeAdjustment (collidedPnt.value(), playerTrsl, tileLines);
                if (!finalVector.has_value() || finalVector.value().SquareMagnitude() > adjustedVec.SquareMagnitude()) {
                    finalVector = adjustedVec;
                }
            }
        }
    }

    // Slight push out of walls
    if (finalVector.has_value() && finalVector.value().SquareMagnitude() > 0.25f) {
        switch (vecQuadrant)
        {
        case phs::Quadrant::I:
            finalVector.value().X() -= 1.f;
            finalVector.value().Y() -= 1.f;
            break;
        case phs::Quadrant::II:
            finalVector.value().X() += 1.f;
            finalVector.value().Y() -= 1.f;
            break;
        case phs::Quadrant::III:
            finalVector.value().X() += 1.f;
            finalVector.value().Y() += 1.f;
            break;
        case phs::Quadrant::IV:
            finalVector.value().X() -= 1.f;
            finalVector.value().Y() += 1.f;
            break;
        case phs::Quadrant::X_ALIGNED:
            finalVector.value().X() -= 1.f;
            break;
        case phs::Quadrant::Y_ALIGNED:
            finalVector.value().Y() -= 1.f;
            break;
        case phs::Quadrant::X_OPPOSITE:
            finalVector.value().X() += 1.f;
            break;
        case phs::Quadrant::Y_OPPOSITE:
            finalVector.value().Y() += 1.f;
            break;
        default:
            break;
        }
    }

    return finalVector;
}

std::optional<phs::Vector2D> GetGroundNormalUnderPlayer (const Collider& playerCollider,
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
        anchorPnt.X() = playerCollider.Max().X() + 1.e-2f;
        anchorPnt.Y() = playerCollider.Max().Y() + 1.f;
        break;
    }
    case phs::Quadrant::II:
    case phs::Quadrant::III:
    case phs::Quadrant::X_OPPOSITE:
    {
        anchorPnt.X() = playerCollider.Min().X() - 1.e-2f;
        anchorPnt.Y() = playerCollider.Max().Y() + 1.f;
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
    return phs::UpVector();
}

bool IsPlayerGrounded (const Collider& playerCollider, const GroundData& groundData)
{
    auto minPnt = playerCollider.Min();
    auto maxPnt = playerCollider.Max();

    float center = minPnt.X() + playerCollider.Width() / 2;
    size_t tile_center = static_cast <size_t> (std::floorf (center / groundData.tile_width));
    size_t tile_right = static_cast <size_t> (std::floorf (maxPnt.X() / groundData.tile_width));
    size_t tile_left = static_cast <size_t> (std::floorf (minPnt.X() / groundData.tile_width));
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

    return static_cast <float> (tile_y) * groundData.tile_height - maxPnt.Y() < 1.2f;
}

}

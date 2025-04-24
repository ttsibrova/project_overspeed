#include <WorldInteraction/GroundCollision.h>

#include <Core/Logger.h>
#include <Geom/Precision.h>
#include <GeomAlgo/LineIntersection.h>
#include <World/Settings.h>

#include <cassert>
#include <print>

#include <Debug/DebugLog.h>

namespace Collision {
using geom::Quadrant;
namespace precision = geom::precision;

namespace {

std::vector<std::pair<geom::Point2D, geom::Point2D>> GetTileLines (geom::Quadrant quadrant, const geom::Point2D& minTileCorner,
                                                                   const geom::Point2D& maxTileCorner)
{
    using namespace geom;
    std::vector<std::pair<Point2D, Point2D>> res;
    switch (quadrant) {
    case Quadrant::I:
        res.emplace_back (Point2D (minTileCorner.x, maxTileCorner.y), maxTileCorner);
        res.emplace_back (Point2D (maxTileCorner.x, minTileCorner.y), maxTileCorner);
        break;
    case Quadrant::II:
        res.emplace_back (Point2D (minTileCorner.x, maxTileCorner.y), maxTileCorner);
        res.emplace_back (minTileCorner, Point2D (minTileCorner.x, maxTileCorner.y));
        break;
    case Quadrant::III:
        res.emplace_back (minTileCorner, Point2D (minTileCorner.x, maxTileCorner.y));
        res.emplace_back (minTileCorner, Point2D (maxTileCorner.x, minTileCorner.y));
        break;
    case Quadrant::IV:
        res.emplace_back (minTileCorner, Point2D (maxTileCorner.x, minTileCorner.y));
        res.emplace_back (Point2D (maxTileCorner.x, minTileCorner.y), maxTileCorner);
        break;
    case Quadrant::X_ALIGNED:
        res.emplace_back (maxTileCorner, Point2D (maxTileCorner.x, minTileCorner.y));
        break;
    case Quadrant::Y_ALIGNED:
        res.emplace_back (maxTileCorner, Point2D (minTileCorner.x, maxTileCorner.y));
        break;
    case Quadrant::X_OPPOSITE:
        res.emplace_back (minTileCorner, Point2D (minTileCorner.x, maxTileCorner.y));
        break;
    case Quadrant::Y_OPPOSITE:
        res.emplace_back (minTileCorner, Point2D (maxTileCorner.x, minTileCorner.y));
        break;
    }
    return res;
}

bool IsBelongToSegment (const geom::Point2D& p, const geom::Point2D& p1, const geom::Point2D& p2)
{
    return p.x >= std::min (p1.x, p2.x) - precision::halfPixel &&
           p.x <= std::max (p1.x, p2.x) + precision::halfPixel &&
           p.y >= std::min (p1.y, p2.y) - precision::halfPixel &&
           p.y <= std::max (p1.y, p2.y) + precision::halfPixel;
}

std::optional<geom::Vector2D> SweepCollision (const physics::Collider& playerCollider, const geom::Vector2D& playerTrsl,
                                              geom::Point2D tileMinCorner, geom::Point2D tileMaxCorner)
{
    float colliderHalfHeight = playerCollider.getHeight() / 2.f;
    float colliderHalfWidth  = playerCollider.getWidth() / 2.f;
    geom::Point2D colliderKeyPnt (playerCollider.getMin().x + colliderHalfWidth, playerCollider.getMin().y + colliderHalfHeight);
    geom::Point2D trajectoryEndPnt = colliderKeyPnt.translated (playerTrsl);

    // std::print ("Collider min pnt - x: {}, y: {}\n", playerCollider.getMin().x, playerCollider.getMin().y);
    // std::print ("Collider max pnt - x: {}, y: {}\n", playerCollider.getMax().x, playerCollider.getMax().y);

    // std::print ("Collider key pnt - x: {}, y: {}\n", colliderKeyPnt.x, colliderKeyPnt.y);
    // std::print ("Trajectory end pnt - x: {}, y: {}\n", trajectoryEndPnt.x, trajectoryEndPnt.y);

    // std::print ("Tile min pnt - x: {}, y: {}\n", tileMinCorner.x, tileMinCorner.y);
    // std::print ("Tile max pnt - x: {}, y: {}\n", tileMaxCorner.x, tileMaxCorner.y);

    tileMinCorner.x -= colliderHalfWidth;
    tileMinCorner.y -= colliderHalfHeight;

    tileMaxCorner.x += colliderHalfWidth;
    tileMaxCorner.y += colliderHalfHeight;

    // std::print ("Updated tile min pnt - x: {}, y: {}\n", tileMinCorner.x, tileMinCorner.y);
    // std::print ("Updated tile max pnt - x: {}, y: {}\n", tileMaxCorner.x, tileMaxCorner.y);

    auto tileLines =
        GetTileLines (geom::getInversedQuadrant (geom::getVectorQudrant (playerTrsl)), tileMinCorner, tileMaxCorner);

    std::optional<geom::Vector2D> newTrsl;
    for (const auto line : tileLines) {
        auto intersectPnt = geom::algo::intersectLines (colliderKeyPnt, trajectoryEndPnt, line.first, line.second);
        if (!intersectPnt.has_value()) {
            core::log::warning("Intersector failed to find value, manual branch DISABLED");
            ////Embracing a specific issue where our vector from chosen point strinctly aligned with
            ////tile side. Selecting the point that is basically closes to original character position
            // Quadrant vQ = physics::getVectorQudrant (playerTrsl);
            // switch (vQ)
            //{
            // case Quadrant::I:
            // case Quadrant::II:
            // case Quadrant::III:
            // case Quadrant::IV:
            //     //TODO update this after adding slopes
            //     break;
            // case Quadrant::X_ALIGNED:
            //{
            //     float minX = std::min ({trajectoryEndPnt.x, line.first.x, line.second.x});
            //     assert (trajectoryEndPnt.y == colliderKeyPnt.y && line.first.y == line.second.y);
            //     intersectPnt = geom::Point2D (minX, trajectoryEndPnt.y);
            //     break;
            // }
            // case Quadrant::Y_ALIGNED:
            //{
            //     float minY = std::min ({trajectoryEndPnt.y, line.first.y, line.second.y});
            //     assert (trajectoryEndPnt.x == colliderKeyPnt.x && line.first.x == line.second.x);
            //     intersectPnt = geom::Point2D (trajectoryEndPnt.x, minY);
            //     break;
            // }
            // case Quadrant::X_OPPOSITE:
            //{
            //     float maxX = std::max ({trajectoryEndPnt.x, line.first.x, line.second.x});
            //     assert (trajectoryEndPnt.y == colliderKeyPnt.y && line.first.y == line.second.y);
            //     intersectPnt = geom::Point2D (maxX, trajectoryEndPnt.y);
            //     break;
            // }
            // case Quadrant::Y_OPPOSITE:
            //{
            //     float maxY = std::max ({trajectoryEndPnt.y, line.first.y, line.second.y});
            //     assert (trajectoryEndPnt.x == colliderKeyPnt.x && line.first.x == line.second.x);
            //     intersectPnt = geom::Point2D (trajectoryEndPnt.x, maxY);
            //     break;
            // }
            // default:
            //     break;
            // }
        }
        else if (!IsBelongToSegment (intersectPnt.value(), colliderKeyPnt, trajectoryEndPnt) ||
                 !IsBelongToSegment (intersectPnt.value(), line.first, line.second)) {
            // std::print ("Intersector found the value x:{}, y:{}, but it was rejected\n", intersectPnt.value().x, intersectPnt.value().y);
            continue;
        }
        debug::log (line, debug::Collision);
        if (intersectPnt.has_value()) {
            // std::print ("Current line - x: {}, y: {} -- x: {}, y: {}\n", line.first.x, line.first.y, line.second.x, line.second.y);
            // std::print ("Intersect pnt - x: {}, y: {}\n", intersectPnt.value().x, intersectPnt.value().y);
        }
        if (!newTrsl.has_value() && intersectPnt.has_value()) {
            newTrsl = geom::Vector2D (intersectPnt.value().x - colliderKeyPnt.x,
                                      intersectPnt.value().y - colliderKeyPnt.y);
        }
        if (newTrsl.has_value() && intersectPnt.has_value()) {
            auto candidate = geom::Vector2D (intersectPnt.value().x - colliderKeyPnt.x,
                                             intersectPnt.value().y - colliderKeyPnt.y);
            if (candidate.getSquareMagnitude() < newTrsl.value().getSquareMagnitude()) {
                newTrsl = candidate;
            }
        }
    }

    if (newTrsl.has_value()) {
        auto  dir       = playerTrsl.normalized();
        float newLength = newTrsl.value().getMagnitude();
        // std::print ("Computed adj vector - x: {}, y: {}\n", newTrsl.value().x, newTrsl.value().y);
        if (newLength < precision::floatTol) {
            newTrsl = geom::Vector2D();
        }
        else {
            newTrsl = dir * newLength;
        }
        // std::print ("Adjusted adj vector - x: {}, y: {}\n", newTrsl.value().x, newTrsl.value().y);
    }
    return newTrsl;
}

} // namespace

std::optional<geom::Vector2D> HitScanGround (const physics::Collider& playerCollider, const geom::Vector2D& playerTrsl,
                                             const world::GroundData& groundData)
{
    physics::Collider playerNewCollider = playerCollider.translated (playerTrsl);
    const map::GridTileSize& tileSize   = world::settings::tileSize;

    auto minPnt = playerNewCollider.getMin();
    auto maxPnt = playerNewCollider.getMax();

    size_t left_tile = static_cast<size_t> (std::floorf ((minPnt.x + precision::quarterPixel) / tileSize.width));
    if (left_tile > groundData.tiles.extent (0) - 1) {
        left_tile = 0U;
    }
    size_t right_tile = static_cast<size_t> (std::floorf ((maxPnt.x - precision::quarterPixel) / tileSize.width));
    right_tile = std::min (right_tile, groundData.tiles.extent (0) - 1);
    size_t top_tile   = static_cast<size_t> (std::floorf ((minPnt.y + precision::quarterPixel) / tileSize.height));
    if (top_tile > groundData.tiles.extent (1) - 1) {
        top_tile = 0U;
    }
    size_t bottom_tile = static_cast<size_t> (std::floorf ((maxPnt.y - precision::quarterPixel) / tileSize.height));
    bottom_tile = std::min (bottom_tile, groundData.tiles.extent (1) - 1);

    std::optional<geom::Vector2D> finalVector;
    for (size_t i = left_tile; i <= right_tile; i++) {
        for (size_t j = top_tile; j <= bottom_tile; j++) {
            if (groundData.tiles[std::array { i, j }] != 0) {
                geom::Point2D tileMinCorner (static_cast<float> (i) * tileSize.width,
                                             static_cast<float> (j) * tileSize.height);
                geom::Point2D tileMaxCorner (tileMinCorner.x + tileSize.width,
                                             tileMinCorner.y + tileSize.height);
                debug::log (std::make_pair (tileMinCorner, tileMaxCorner), debug::Collision);
                auto adjustedVec = SweepCollision (playerCollider, playerTrsl, tileMinCorner, tileMaxCorner);
                if (!finalVector.has_value() ||
                    finalVector.has_value() && adjustedVec.has_value() &&
                        finalVector.value().getSquareMagnitude() > adjustedVec.value().getSquareMagnitude()) {
                    finalVector = adjustedVec;
                }
            }
        }
    }

    return finalVector;
}

bool IsPlayerCollidesWithGround (const physics::Collider& playerCollider, const world::GroundData& groundData)
{
    auto minPnt = playerCollider.getMin();
    auto maxPnt = playerCollider.getMax();

    const map::GridTileSize& tileSize = world::settings::tileSize;

    size_t left_tile = static_cast<size_t> (std::floorf ((minPnt.x + precision::quarterPixel) / tileSize.width));
    if (left_tile > groundData.tiles.extent (0) - 1) {
        left_tile = 0U;
    }
    size_t right_tile = static_cast<size_t> (std::floorf ((maxPnt.x - precision::quarterPixel) / tileSize.width));
    right_tile = std::min (right_tile, groundData.tiles.extent (0) - 1);
    size_t top_tile   = static_cast<size_t> (std::floorf ((minPnt.y + precision::quarterPixel) / tileSize.height));
    if (top_tile > groundData.tiles.extent (1) - 1) {
        top_tile = 0U;
    }
    size_t bottom_tile = static_cast<size_t> (std::floorf ((maxPnt.y - precision::quarterPixel) / tileSize.height));
    bottom_tile = std::min (bottom_tile, groundData.tiles.extent (1) - 1);

    for (size_t i = left_tile; i <= right_tile; i++) {
        for (size_t j = top_tile; j <= bottom_tile; j++) {
            if (groundData.tiles[std::array { i, j }] != 0) {
                return true;
            }
        }
    }
    return false;
}

std::optional<geom::Vector2D> GetGroundNormalUnderPlayer (const physics::Collider& playerCollider,
                                                          Quadrant playerVelocityQuadrant, const world::GroundData& groundData)
{
    geom::Point2D anchorPnt;
    switch (playerVelocityQuadrant) {
    case Quadrant::I:
    case Quadrant::IV:
    case Quadrant::X_ALIGNED:
    {
        anchorPnt.x = playerCollider.getMax().x + precision::pixel + precision::quarterPixel;
        anchorPnt.y = playerCollider.getMax().y + precision::pixel;
        break;
    }
    case Quadrant::II:
    case Quadrant::III:
    case Quadrant::X_OPPOSITE:
    {
        anchorPnt.x = playerCollider.getMin().x - precision::pixel + precision::quarterPixel;
        anchorPnt.y = playerCollider.getMax().y + precision::pixel;
        break;
    }
    case Quadrant::Y_ALIGNED:
    case Quadrant::Y_OPPOSITE:
    {
        assert (false); // Is this really possible case?
        return std::nullopt;
    }
    }

    size_t tile_x = static_cast<size_t> (std::floorf (anchorPnt.x / world::settings::tileSize.width));
    size_t tile_y = static_cast<size_t> (std::floorf (anchorPnt.y / world::settings::tileSize.height));

    tile_x = std::min (tile_x, groundData.tiles.extent (0) - 1);
    tile_y = std::min (tile_y, groundData.tiles.extent (1) - 1);

    if (tile_y == 0) {
        return std::nullopt;
    }

    if (groundData.tiles[std::array { tile_x, tile_y }] == 0 ||   // block is not a ground
        groundData.tiles[std::array { tile_x, tile_y - 1 }] != 0) // there is a block on top, not a ground case
    {
        return std::nullopt;
    }

    // TODO return vector should depend on block type
    return geom::getUpVector();
}

bool IsPlayerGrounded (const physics::Collider& playerCollider, const world::GroundData& groundData)
{
    auto minPnt = playerCollider.getMin();
    auto maxPnt = playerCollider.getMax();

    const map::GridTileSize& tileSize = world::settings::tileSize;

    float  center      = minPnt.x + playerCollider.getWidth() / 2;
    size_t tile_center = static_cast<size_t> (std::floorf (center / tileSize.width));
    size_t tile_right  = static_cast<size_t> (std::floorf ((maxPnt.x - precision::quarterPixel) / tileSize.width));
    size_t tile_left = static_cast<size_t> (std::floorf ((minPnt.x + precision::quarterPixel) / tileSize.width));
    size_t tile_y      = static_cast<size_t> (std::ceilf (maxPnt.y / tileSize.height));
    tile_y        = std::min (groundData.tiles.extent (1) - 1, tile_y);
    tile_center   = std::min (groundData.tiles.extent (0) - 1, tile_center);
    tile_right    = std::min (groundData.tiles.extent (0) - 1, tile_right);
    tile_left     = std::min (groundData.tiles.extent (0) - 1, tile_left);

    if (groundData.tiles[std::array { tile_left, tile_y }] == 0 && groundData.tiles[std::array { tile_center, tile_y }] == 0 &&
        groundData.tiles[std::array { tile_right, tile_y }] == 0) {
        return false;
    }

    //debug::log ("Distance to ground: {}", static_cast<float> (tile_y) * tileSize.height - maxPnt.y);

    return static_cast<float> (tile_y) * tileSize.height - maxPnt.y < precision::quarterPixel;
}

} // namespace Collision

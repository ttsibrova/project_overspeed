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
        res.emplace_back (Point2D (minTileCorner.X(), maxTileCorner.Y()), maxTileCorner);
        res.emplace_back (Point2D (maxTileCorner.X(), minTileCorner.Y()), maxTileCorner);
        break;
    case Quadrant::II:
        res.emplace_back (Point2D (minTileCorner.X(), maxTileCorner.Y()), maxTileCorner);
        res.emplace_back (minTileCorner, Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case Quadrant::III:
        res.emplace_back (minTileCorner, Point2D (minTileCorner.X(), maxTileCorner.Y()));
        res.emplace_back (minTileCorner, Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    case Quadrant::IV:
        res.emplace_back (minTileCorner, Point2D (maxTileCorner.X(), minTileCorner.Y()));
        res.emplace_back (Point2D (maxTileCorner.X(), minTileCorner.Y()), maxTileCorner);
        break;
    case Quadrant::X_ALIGNED:
        res.emplace_back (maxTileCorner, Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    case Quadrant::Y_ALIGNED:
        res.emplace_back (maxTileCorner, Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case Quadrant::X_OPPOSITE:
        res.emplace_back (minTileCorner, Point2D (minTileCorner.X(), maxTileCorner.Y()));
        break;
    case Quadrant::Y_OPPOSITE:
        res.emplace_back (minTileCorner, Point2D (maxTileCorner.X(), minTileCorner.Y()));
        break;
    }
    return res;
}

bool IsBelongToSegment (const geom::Point2D& p, const geom::Point2D& p1, const geom::Point2D& p2)
{
    return p.X() >= std::min (p1.X(), p2.X()) - precision::half_pixel &&
           p.X() <= std::max (p1.X(), p2.X()) + precision::half_pixel &&
           p.Y() >= std::min (p1.Y(), p2.Y()) - precision::half_pixel &&
           p.Y() <= std::max (p1.Y(), p2.Y()) + precision::half_pixel;
}

std::optional<geom::Vector2D> SweepCollision (const physics::Collider& playerCollider, const geom::Vector2D& playerTrsl,
                                              geom::Point2D tileMinCorner, geom::Point2D tileMaxCorner)
{
    float colliderHalfHeight = playerCollider.Height() / 2.f;
    float colliderHalfWidth  = playerCollider.Width() / 2.f;
    geom::Point2D colliderKeyPnt (playerCollider.Min().X() + colliderHalfWidth, playerCollider.Min().Y() + colliderHalfHeight);
    geom::Point2D trajectoryEndPnt = colliderKeyPnt.Translated (playerTrsl);

    // std::print ("Collider min pnt - X: {}, Y: {}\n", playerCollider.Min().X(), playerCollider.Min().Y());
    // std::print ("Collider max pnt - X: {}, Y: {}\n", playerCollider.Max().X(), playerCollider.Max().Y());

    // std::print ("Collider key pnt - X: {}, Y: {}\n", colliderKeyPnt.X(), colliderKeyPnt.Y());
    // std::print ("Trajectory end pnt - X: {}, Y: {}\n", trajectoryEndPnt.X(), trajectoryEndPnt.Y());

    // std::print ("Tile min pnt - X: {}, Y: {}\n", tileMinCorner.X(), tileMinCorner.Y());
    // std::print ("Tile max pnt - X: {}, Y: {}\n", tileMaxCorner.X(), tileMaxCorner.Y());

    tileMinCorner.X() -= colliderHalfWidth;
    tileMinCorner.Y() -= colliderHalfHeight;

    tileMaxCorner.X() += colliderHalfWidth;
    tileMaxCorner.Y() += colliderHalfHeight;

    // std::print ("Updated tile min pnt - X: {}, Y: {}\n", tileMinCorner.X(), tileMinCorner.Y());
    // std::print ("Updated tile max pnt - X: {}, Y: {}\n", tileMaxCorner.X(), tileMaxCorner.Y());

    auto tileLines =
        GetTileLines (geom::getInversedQuadrant (geom::getVectorQudrant (playerTrsl)), tileMinCorner, tileMaxCorner);

    std::optional<geom::Vector2D> newTrsl;
    for (const auto line : tileLines) {
        auto intersectPnt = geom::algo::IntersectLines (colliderKeyPnt, trajectoryEndPnt, line.first, line.second);
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
            //     float minX = std::min ({trajectoryEndPnt.X(), line.first.X(), line.second.X()});
            //     assert (trajectoryEndPnt.Y() == colliderKeyPnt.Y() && line.first.Y() == line.second.Y());
            //     intersectPnt = geom::Point2D (minX, trajectoryEndPnt.Y());
            //     break;
            // }
            // case Quadrant::Y_ALIGNED:
            //{
            //     float minY = std::min ({trajectoryEndPnt.Y(), line.first.Y(), line.second.Y()});
            //     assert (trajectoryEndPnt.X() == colliderKeyPnt.X() && line.first.X() == line.second.X());
            //     intersectPnt = geom::Point2D (trajectoryEndPnt.X(), minY);
            //     break;
            // }
            // case Quadrant::X_OPPOSITE:
            //{
            //     float maxX = std::max ({trajectoryEndPnt.X(), line.first.X(), line.second.X()});
            //     assert (trajectoryEndPnt.Y() == colliderKeyPnt.Y() && line.first.Y() == line.second.Y());
            //     intersectPnt = geom::Point2D (maxX, trajectoryEndPnt.Y());
            //     break;
            // }
            // case Quadrant::Y_OPPOSITE:
            //{
            //     float maxY = std::max ({trajectoryEndPnt.Y(), line.first.Y(), line.second.Y()});
            //     assert (trajectoryEndPnt.X() == colliderKeyPnt.X() && line.first.X() == line.second.X());
            //     intersectPnt = geom::Point2D (trajectoryEndPnt.X(), maxY);
            //     break;
            // }
            // default:
            //     break;
            // }
        }
        else if (!IsBelongToSegment (intersectPnt.value(), colliderKeyPnt, trajectoryEndPnt) ||
                 !IsBelongToSegment (intersectPnt.value(), line.first, line.second)) {
            // std::print ("Intersector found the value X:{}, Y:{}, but it was rejected\n", intersectPnt.value().X(), intersectPnt.value().Y());
            continue;
        }
        Debug::Log (line, Debug::Collision);
        if (intersectPnt.has_value()) {
            // std::print ("Current line - X: {}, Y: {} -- X: {}, Y: {}\n", line.first.X(), line.first.Y(), line.second.X(), line.second.Y());
            // std::print ("Intersect pnt - X: {}, Y: {}\n", intersectPnt.value().X(), intersectPnt.value().Y());
        }
        if (!newTrsl.has_value() && intersectPnt.has_value()) {
            newTrsl = geom::Vector2D (intersectPnt.value().X() - colliderKeyPnt.X(),
                                      intersectPnt.value().Y() - colliderKeyPnt.Y());
        }
        if (newTrsl.has_value() && intersectPnt.has_value()) {
            auto candidate = geom::Vector2D (intersectPnt.value().X() - colliderKeyPnt.X(),
                                             intersectPnt.value().Y() - colliderKeyPnt.Y());
            if (candidate.SquareMagnitude() < newTrsl.value().SquareMagnitude()) {
                newTrsl = candidate;
            }
        }
    }

    if (newTrsl.has_value()) {
        auto  dir       = playerTrsl.Normalized();
        float newLength = newTrsl.value().Magnitude();
        // std::print ("Computed adj vector - X: {}, Y: {}\n", newTrsl.value().X(), newTrsl.value().Y());
        if (newLength < precision::float_tol) {
            newTrsl = geom::Vector2D();
        }
        else {
            newTrsl = dir * newLength;
        }
        // std::print ("Adjusted adj vector - X: {}, Y: {}\n", newTrsl.value().X(), newTrsl.value().Y());
    }
    return newTrsl;
}

} // namespace

std::optional<geom::Vector2D> HitScanGround (const physics::Collider& playerCollider, const geom::Vector2D& playerTrsl,
                                             const world::GroundData& groundData)
{
    physics::Collider playerNewCollider = playerCollider.Translated (playerTrsl);
    const map::GridTileSize& tileSize   = world::settings::tileSize;

    auto minPnt = playerNewCollider.Min();
    auto maxPnt = playerNewCollider.Max();

    size_t left_tile = static_cast<size_t> (std::floorf ((minPnt.X() + precision::quarter_pixel) / tileSize.width));
    if (left_tile > groundData.tiles.extent (0) - 1) {
        left_tile = 0U;
    }
    size_t right_tile = static_cast<size_t> (std::floorf ((maxPnt.X() - precision::quarter_pixel) / tileSize.width));
    right_tile = std::min (right_tile, groundData.tiles.extent (0) - 1);
    size_t top_tile   = static_cast<size_t> (std::floorf ((minPnt.Y() + precision::quarter_pixel) / tileSize.height));
    if (top_tile > groundData.tiles.extent (1) - 1) {
        top_tile = 0U;
    }
    size_t bottom_tile = static_cast<size_t> (std::floorf ((maxPnt.Y() - precision::quarter_pixel) / tileSize.height));
    bottom_tile = std::min (bottom_tile, groundData.tiles.extent (1) - 1);

    std::optional<geom::Vector2D> finalVector;
    for (size_t i = left_tile; i <= right_tile; i++) {
        for (size_t j = top_tile; j <= bottom_tile; j++) {
            if (groundData.tiles[std::array { i, j }] != 0) {
                geom::Point2D tileMinCorner (static_cast<float> (i) * tileSize.width,
                                             static_cast<float> (j) * tileSize.height);
                geom::Point2D tileMaxCorner (tileMinCorner.X() + tileSize.width,
                                             tileMinCorner.Y() + tileSize.height);
                Debug::Log (std::make_pair (tileMinCorner, tileMaxCorner), Debug::Collision);
                auto adjustedVec = SweepCollision (playerCollider, playerTrsl, tileMinCorner, tileMaxCorner);
                if (!finalVector.has_value() ||
                    finalVector.has_value() && adjustedVec.has_value() &&
                        finalVector.value().SquareMagnitude() > adjustedVec.value().SquareMagnitude()) {
                    finalVector = adjustedVec;
                }
            }
        }
    }

    return finalVector;
}

bool IsPlayerCollidesWithGround (const physics::Collider& playerCollider, const world::GroundData& groundData)
{
    auto minPnt = playerCollider.Min();
    auto maxPnt = playerCollider.Max();

    const map::GridTileSize& tileSize = world::settings::tileSize;

    size_t left_tile = static_cast<size_t> (std::floorf ((minPnt.X() + precision::quarter_pixel) / tileSize.width));
    if (left_tile > groundData.tiles.extent (0) - 1) {
        left_tile = 0U;
    }
    size_t right_tile = static_cast<size_t> (std::floorf ((maxPnt.X() - precision::quarter_pixel) / tileSize.width));
    right_tile = std::min (right_tile, groundData.tiles.extent (0) - 1);
    size_t top_tile   = static_cast<size_t> (std::floorf ((minPnt.Y() + precision::quarter_pixel) / tileSize.height));
    if (top_tile > groundData.tiles.extent (1) - 1) {
        top_tile = 0U;
    }
    size_t bottom_tile = static_cast<size_t> (std::floorf ((maxPnt.Y() - precision::quarter_pixel) / tileSize.height));
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
        anchorPnt.X() = playerCollider.Max().X() + precision::pixel + precision::quarter_pixel;
        anchorPnt.Y() = playerCollider.Max().Y() + precision::pixel;
        break;
    }
    case Quadrant::II:
    case Quadrant::III:
    case Quadrant::X_OPPOSITE:
    {
        anchorPnt.X() = playerCollider.Min().X() - precision::pixel + precision::quarter_pixel;
        anchorPnt.Y() = playerCollider.Max().Y() + precision::pixel;
        break;
    }
    case Quadrant::Y_ALIGNED:
    case Quadrant::Y_OPPOSITE:
    {
        assert (false); // Is this really possible case?
        return std::nullopt;
    }
    }

    size_t tile_x = static_cast<size_t> (std::floorf (anchorPnt.X() / world::settings::tileSize.width));
    size_t tile_y = static_cast<size_t> (std::floorf (anchorPnt.Y() / world::settings::tileSize.height));

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
    auto minPnt = playerCollider.Min();
    auto maxPnt = playerCollider.Max();

    const map::GridTileSize& tileSize = world::settings::tileSize;

    float  center      = minPnt.X() + playerCollider.Width() / 2;
    size_t tile_center = static_cast<size_t> (std::floorf (center / tileSize.width));
    size_t tile_right  = static_cast<size_t> (std::floorf ((maxPnt.X() - precision::quarter_pixel) / tileSize.width));
    size_t tile_left = static_cast<size_t> (std::floorf ((minPnt.X() + precision::quarter_pixel) / tileSize.width));
    size_t tile_y      = static_cast<size_t> (std::ceilf (maxPnt.Y() / tileSize.height));
    tile_y        = std::min (groundData.tiles.extent (1) - 1, tile_y);
    tile_center   = std::min (groundData.tiles.extent (0) - 1, tile_center);
    tile_right    = std::min (groundData.tiles.extent (0) - 1, tile_right);
    tile_left     = std::min (groundData.tiles.extent (0) - 1, tile_left);

    if (groundData.tiles[std::array { tile_left, tile_y }] == 0 && groundData.tiles[std::array { tile_center, tile_y }] == 0 &&
        groundData.tiles[std::array { tile_right, tile_y }] == 0) {
        return false;
    }

    Debug::Log (std::format ("Distance to ground: {}", static_cast<float> (tile_y) * tileSize.height - maxPnt.Y()));

    return static_cast<float> (tile_y) * tileSize.height - maxPnt.Y() < precision::quarter_pixel;
}

} // namespace Collision

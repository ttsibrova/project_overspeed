#include <WorldInteraction/GroundCollision.h>

#include <Core/Logger.h>
#include <Geom/LineSegment.h>
#include <Geom/Precision.h>
#include <GeomAlgo/LineIntersection.h>
#include <World/Settings.h>

#include <cassert>
#include <print>
#include <tuple>

#include <Debug/DebugLog.h>

namespace collision {
using geom::Quadrant;
namespace precision = geom::precision;

namespace {

struct GroundBounds
{
    size_t left;
    size_t right;
    size_t top;
    size_t bottom;
};

GroundBounds getPlayerPositionBounds (physics::Collider playerCollider, const world::GroundData& groundData)
{
    playerCollider.extend (-precision::quarterPixel);
    const map::GridTileSize& tileSize = world::settings::tileSize;

    const auto minPnt = playerCollider.getMin();
    const auto maxPnt = playerCollider.getMax();

    const size_t lastRightTile  = groundData.tiles.extent (0) - 1;
    const size_t lastBottomTile = groundData.tiles.extent (1) - 1;

    size_t leftBound   = static_cast<size_t> (std::floorf (minPnt.x / tileSize.width));
    leftBound          = leftBound > lastRightTile ? 0U : leftBound;
    size_t rightBound  = static_cast<size_t> (std::floorf (maxPnt.x / tileSize.width));
    rightBound         = std::min (rightBound, lastRightTile);
    size_t topBound    = static_cast<size_t> (std::floorf (minPnt.y / tileSize.height));
    topBound           = topBound > lastBottomTile ? 0U : topBound;
    size_t bottomBound = static_cast<size_t> (std::floorf (maxPnt.y / tileSize.height));
    bottomBound        = std::min (bottomBound, lastBottomTile);

    return {
        .left   = leftBound,
        .right  = rightBound,
        .top    = topBound,
        .bottom = bottomBound,
    };
}

std::vector<geom::LineSegment> getTileLines (const geom::Quadrant quadrant, //
                                             const geom::Point2D& minTileCorner,
                                             const geom::Point2D& maxTileCorner)
{
    using namespace geom;
    std::vector<LineSegment> res;
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
    case Quadrant::X_ALIGNED : res.emplace_back (maxTileCorner, Point2D (maxTileCorner.x, minTileCorner.y)); break;
    case Quadrant::Y_ALIGNED : res.emplace_back (maxTileCorner, Point2D (minTileCorner.x, maxTileCorner.y)); break;
    case Quadrant::X_OPPOSITE: res.emplace_back (minTileCorner, Point2D (minTileCorner.x, maxTileCorner.y)); break;
    case Quadrant::Y_OPPOSITE: res.emplace_back (minTileCorner, Point2D (maxTileCorner.x, minTileCorner.y)); break;
    }
    return res;
}

std::optional<geom::Vector2D> computeSweepCollision (const physics::Collider& playerCollider,
                                                     const geom::Vector2D&    playerTrsl,
                                                     geom::Point2D            tileMinCorner,
                                                     geom::Point2D            tileMaxCorner)
{
    const float         colliderHalfHeight = playerCollider.getHeight() / 2.f;
    const float         colliderHalfWidth  = playerCollider.getWidth() / 2.f;
    const geom::Point2D colliderKeyPnt (playerCollider.getMin().x + colliderHalfWidth,
                                        playerCollider.getMin().y + colliderHalfHeight);

    const geom::LineSegment trajectorySegment {
        .first = colliderKeyPnt,
        .last  = colliderKeyPnt.translated (playerTrsl),
    };

    tileMinCorner.x -= colliderHalfWidth;
    tileMinCorner.y -= colliderHalfHeight;

    tileMaxCorner.x += colliderHalfWidth;
    tileMaxCorner.y += colliderHalfHeight;

    const auto tileSegments = getTileLines (geom::getInversedQuadrant (geom::getVectorQudrant (playerTrsl)),
                                            tileMinCorner,
                                            tileMaxCorner);

    std::optional<geom::Vector2D> newTrsl;
    for (const auto segment : tileSegments) {
        const auto intersectPnt = geom::algo::intersectSegments (trajectorySegment, segment);
        if (!intersectPnt.has_value()) {
            core::log::warning ("Intersector failed to find value, manual branch DISABLED");
            continue;
        }
        debug::log (segment, debug::Collision);
        if (intersectPnt.has_value()) {
            if (newTrsl.has_value()) {
                const auto candidate = geom::Vector2D {
                    .x = intersectPnt->x - colliderKeyPnt.x,
                    .y = intersectPnt->y - colliderKeyPnt.y,
                };
                if (candidate.getSquareMagnitude() < newTrsl->getSquareMagnitude()) {
                    newTrsl = candidate;
                }
            }
            else {
                newTrsl = geom::Vector2D {
                    .x = intersectPnt->x - colliderKeyPnt.x,
                    .y = intersectPnt->y - colliderKeyPnt.y,
                };
            }
        }
    }

    if (newTrsl.has_value()) {
        const auto  dir       = playerTrsl.normalized();
        const float newLength = newTrsl.value().getMagnitude();
        newTrsl               = newLength < precision::floatTol ? geom::Vector2D{} : dir * newLength;
    }
    return newTrsl;
}

} // namespace

std::optional<geom::Vector2D> computeTranslationBeforeCollision (const physics::Collider& playerCollider,
                                             const geom::Vector2D&    playerTrsl,
                                             const world::GroundData& groundData)
{
    GroundBounds bounds = getPlayerPositionBounds (playerCollider.translated (playerTrsl), groundData);
    const map::GridTileSize& tileSize = world::settings::tileSize;

    std::optional<geom::Vector2D> finalVector;
    for (size_t i = bounds.left; i <= bounds.right; i++) {
        for (size_t j = bounds.top; j <= bounds.bottom; j++) {
            if (groundData.tiles[i, j] == map::Tileset::getEmptyTileId()) {
                continue;
            }
            const geom::Point2D tileMinCorner {
                .x = static_cast<float> (i) * tileSize.width,
                .y = static_cast<float> (j) * tileSize.height
            };
            const geom::Point2D tileMaxCorner {
                .x = tileMinCorner.x + tileSize.width,
                .y = tileMinCorner.y + tileSize.height,
            };
            debug::log (geom::LineSegment { tileMinCorner, tileMaxCorner }, debug::Collision);
            const auto adjustedVec = computeSweepCollision (playerCollider, playerTrsl, tileMinCorner, tileMaxCorner);
            if (!finalVector.has_value() || finalVector.has_value() && adjustedVec.has_value() &&
                                            finalVector->getSquareMagnitude() > adjustedVec->getSquareMagnitude()) {
                finalVector = adjustedVec;
            }
        }
    }

    return finalVector;
}

bool isPlayerCollidesWithGround (const physics::Collider& playerCollider, const world::GroundData& groundData)
{
    GroundBounds bounds = getPlayerPositionBounds (playerCollider, groundData);
    const map::GridTileSize& tileSize = world::settings::tileSize;

    for (size_t i = bounds.left; i <= bounds.right; i++) {
        for (size_t j = bounds.top; j <= bounds.bottom; j++) {
            if (groundData.tiles[i, j] != map::Tileset::getEmptyTileId()) {
                return true;
            }
        }
    }
    return false;
}

bool isPlayerGrounded (physics::Collider playerCollider, const world::GroundData& groundData)
{
    playerCollider.extend (geom::Vector2D { .x = -precision::quarterPixel, .y = 0.f });

    const auto minPnt = playerCollider.getMin();
    const auto maxPnt = playerCollider.getMax();

    const size_t lastRightTile = groundData.tiles.extent (0) - 1;
    const size_t lastBottomTile = groundData.tiles.extent (1) - 1;

    const map::GridTileSize& tileSize = world::settings::tileSize;

    float  playerCenter = minPnt.x + playerCollider.getWidth() / 2;
    size_t centerTile   = static_cast<size_t> (std::floorf (playerCenter / tileSize.width));
    size_t rightTile    = static_cast<size_t> (std::floorf (maxPnt.x / tileSize.width));
    size_t leftTile     = static_cast<size_t> (std::floorf (minPnt.x / tileSize.width));
    size_t bottomTile   = static_cast<size_t> (std::ceilf (maxPnt.y / tileSize.height));
    bottomTile          = std::min (lastBottomTile, bottomTile);
    centerTile          = std::min (lastRightTile, centerTile);
    rightTile           = std::min (lastRightTile, rightTile);
    leftTile            = std::min (lastRightTile, leftTile);

    if (groundData.tiles[leftTile, bottomTile] == 0 &&   //
        groundData.tiles[centerTile, bottomTile] == 0 && //
        groundData.tiles[rightTile, bottomTile] == 0) {
        return false;
    }

    return (static_cast<float> (bottomTile) * tileSize.height - maxPnt.y) < precision::quarterPixel;
}

} // namespace collision

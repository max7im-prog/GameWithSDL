#include "segment.hpp"
#include "box2d/collision.h"
#include "box2d/types.h"
#include <box2d/box2d.h>

Segment::Segment(entt::registry &registry, const World &world,
               const SegmentConfig &config)
    : Shape(registry) {
  bodyId = b2CreateBody(world.getWorldId(), &(config.bodyDef));

  b2Segment segment;
  segment.point1 = config.localPoint1;
  segment.point2 = config.localPoint2;
  shapeId = b2CreateSegmentShape(bodyId, &(config.shapeDef), &segment);
}

SegmentConfig SegmentConfig::defaultConfig() {
  SegmentConfig ret;
  ret.shapeDef = b2DefaultShapeDef();
  ret.bodyDef = b2DefaultBodyDef();
  ret.localPoint1 = {0,0};
  ret.localPoint2 = {0,0};
  return ret;
}

b2Vec2 Segment::getPoint1() {
  b2Segment segment = b2Shape_GetSegment(shapeId);
  return b2Body_GetWorldPoint(bodyId, segment.point1);
}

b2Vec2 Segment::getPoint2() {
  b2Segment segment = b2Shape_GetSegment(shapeId);
  return b2Body_GetWorldPoint(bodyId, segment.point2);
}

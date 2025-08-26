#include "shapeFactory.hpp"
#include "circle.hpp"
std::shared_ptr<Circle> ShapeFactory::createCircle(const CircleConfig &config) {
  return create<Circle>(config);
}
std::shared_ptr<Polygon>
ShapeFactory::createPolygon(const PolygonConfig &config) {
  return create<Polygon>(config);
}
std::shared_ptr<Capsule>
ShapeFactory::createCapsule(const CapsuleConfig &config) {
  return create<Capsule>(config);
}
std::shared_ptr<Segment>
ShapeFactory::createSegment(const SegmentConfig &config) {
  return create<Segment>(config);
}
std::shared_ptr<EmptyShape>
ShapeFactory::createEmptyShape(const EmptyShapeConfig &config) {
  return create<EmptyShape>(config);
}
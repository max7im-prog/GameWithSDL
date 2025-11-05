#include "segmentBody.hpp"
#include "body.hpp"
#include "segment.hpp"

SegmentBody::SegmentBody(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const SegmentBodyConfig &config,
                         const std::shared_ptr<ShapeFactory> shapeFactory,
                         const std::shared_ptr<JointFactory> jointFactory)
    : Body(registry, world) {
  segment = shapeFactory->create<Segment>(config.shapeCfg);
  registerChild(segment);
}

void SegmentBodyConfig::defaultConfig() {
  shapeCfg.defaultConfig();
}


const std::shared_ptr<Segment> SegmentBody::getSegment() {

  auto ret = segment.lock();
  if (!ret)
    throw std::runtime_error("Shape expired");
  return ret;
}
#include "segmentBody.hpp"
#include "body.hpp"
#include "segment.hpp"

SegmentBody::SegmentBody(entt::registry &registry,
                         const std::shared_ptr<World> world,
                         const SegmentBodyConfig &config,
                         const std::shared_ptr<PhysicsFactory> physicsFactory)
    : Body(registry, world) {
  segment = physicsFactory->createSegment(config.shapeCfg);
  registerChild(segment);
}

SegmentBodyConfig SegmentBodyConfig::defaultConfig() {
  SegmentBodyConfig ret;
  ret.shapeCfg = SegmentConfig::defaultConfig();
  return ret;
}

const std::shared_ptr<Segment> SegmentBody::getSegment(){
  return segment;
}
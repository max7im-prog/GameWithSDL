#pragma once
#include "bodyPart.hpp"
#include "segment.hpp"

struct SegmentBodyConfig : public BodyConfig {
  static SegmentBodyConfig defaultConfig();
  SegmentConfig shapeCfg;
};

class SegmentBody : public Body {
public:
  const std::shared_ptr<Segment> getSegment();

protected:
  SegmentBody() = delete;
  SegmentBody(entt::registry &registry, const std::shared_ptr<World> world,
              const SegmentBodyConfig &config,
              const std::shared_ptr<PhysicsFactory> physicsFactory);

  std::shared_ptr<Segment> segment;

  friend class BodyFactory;
};
#pragma once
#include "body.hpp"
#include "jointFactory.hpp"
#include "segment.hpp"
#include "shapeFactory.hpp"

struct SegmentBodyConfig : public BodyConfig {
  static SegmentBodyConfig defaultConfig();
  SegmentConfig shapeCfg;
};

class SegmentBody : public Body {
public:
  using Config = SegmentBodyConfig;
  const std::shared_ptr<Segment> getSegment();

protected:
  SegmentBody() = delete;
  SegmentBody(entt::registry &registry, const std::shared_ptr<World> world,
              const SegmentBodyConfig &config,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);

  std::shared_ptr<Segment> segment;

  friend class BodyFactory;
};
#pragma once
#include "body.hpp"
#include "jointFactory.hpp"
#include "segment.hpp"
#include "shapeFactory.hpp"

class SegmentBody : public Body, public VisitableImpl<SegmentBody> {
public:
  struct Config : public Body::Config {
    void defaultConfig() override;
    SegmentConfig shapeCfg;
  };
  const std::shared_ptr<Segment> getSegment();
  virtual b2Vec2 getWorldPos() override;

protected:
  SegmentBody() = delete;
  SegmentBody(entt::registry &registry, const std::shared_ptr<World> world,
              const SegmentBody::Config &config,
              const std::shared_ptr<ShapeFactory> shapeFactory,
              const std::shared_ptr<JointFactory> jointFactory);

  std::weak_ptr<Segment> segment;

  friend class BodyFactory;
};

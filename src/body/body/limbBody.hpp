#pragma once
#include "PIDVectorController.hpp"
#include "body.hpp"
#include "capsule.hpp"
#include "revoluteJoint.hpp"

struct LimbSegmentConfig {
  float len = 1;
  float radius = 0.25;
};

struct LimbControlConfig {
  float KPMultiplier = 1;
  float KIMultiplier = 1;
  float KDMultiplier = 1;
  float maxForceMultiplier = 1;
};

struct LimbBodyConfig : public BodyConfig {
  static LimbBodyConfig defaultConfig();
  CapsuleConfig templateCapsuleConfig;
  RevoluteJointConfig templateJointConfig;
  LimbControlConfig limbControlConfig;

  std::vector<LimbSegmentConfig> segments;
  b2Vec2 basePos;
  b2Rot rotation;
};

class LimbBody : public Body {
public:
  b2Vec2 getBasePos();
  b2Vec2 getEndPos();
  float getLength();
  const std::vector<float> &getSegmentLengths();
  const std::vector<std::shared_ptr<Capsule>> &getSegments() const;
  std::vector<b2Vec2> getJointsPos();

  void setTracking(b2Vec2 worldPoint, bool isTracking);
  bool getTracking();
  b2Vec2 getTrackingPoint();

  void update(float dt) override;

protected:
  LimbBody() = delete;
  LimbBody(entt::registry &registry, const std::shared_ptr<World> world,
           const LimbBodyConfig &config,
           const std::shared_ptr<PhysicsFactory> physicsFactory);

  struct SegmentController {
    PIDVectorController baseController;
    PIDVectorController endController;
  };

  std::vector<std::shared_ptr<Capsule>> segments;
  std::vector<std::shared_ptr<RevoluteJoint>> joints;
  std::vector<SegmentController> controllers;
  std::vector<float> segmentLengths;
  float length;
  const std::vector<LimbSegmentConfig> segmentsConfig;
  struct TrackingContext {
    b2Vec2 trackingPoint = b2Vec2(0, 0);
    bool isTracking = false;
  } trackingContext;

  friend class BodyFactory;
};
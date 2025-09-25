#pragma once
#include "PIDVectorController.hpp"
#include "body.hpp"
#include "box2d/math_functions.h"
#include "capsule.hpp"
#include "connectionFactory.hpp"
#include "jointFactory.hpp"
#include "kinematicUtils.hpp"
#include "revoluteJoint.hpp"
#include "shapeFactory.hpp"

struct LimbSegmentConfig {
  b2Vec2 endPos = {0, 0};
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

  std::vector<AngleConstraint> initialAngleConstraints = {};
  bool enableAngleConstraints = false;
  b2Rot rootRot;
};

class LimbBody : public Body {
public:
  using Config = LimbBodyConfig;
  b2Vec2 getBasePos();
  b2Vec2 getEndPos();
  float getLength();
  void setAngleConstraints(const std::vector<AngleConstraint> & constraints);
  const std::vector<float> &getSegmentLengths();
  const std::vector<std::shared_ptr<Capsule>> &getSegments() const;
  std::vector<b2Vec2> getJointsPos();

  std::shared_ptr<RevoluteConnection> connect(std::shared_ptr<ConnectionFactory> factory, b2Vec2 localPos, b2BodyId attachedBodyId, std::shared_ptr<Body> parentBody);

  void setTracking(b2Vec2 worldPoint, bool isTracking);
  bool getTracking();
  b2Vec2 getTrackingPoint();

  void update(float dt) override;

protected:
  LimbBody() = delete;
  LimbBody(entt::registry &registry, const std::shared_ptr<World> world,
           const LimbBodyConfig &config,
           const std::shared_ptr<ShapeFactory> shapeFactory,
           const std::shared_ptr<JointFactory> jointFactory);

  struct SegmentController {
    PIDVectorController baseController;
    PIDVectorController endController;
  };

  std::vector<std::shared_ptr<Capsule>> segments;
  std::vector<std::shared_ptr<RevoluteJoint>> joints;
  std::vector<SegmentController> controllers;
  std::vector<float> segmentLengths;
  float length;

  struct TrackingContext {
    b2Vec2 trackingPoint = b2Vec2(0, 0);
    bool isTracking = false;
  } trackingContext;

  IKTask rootIKTask;

  const Config config;

  std::weak_ptr<RevoluteConnection> connection;

  friend class BodyFactory;
};
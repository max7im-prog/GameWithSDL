#include "PIDVectorController.hpp"
#include "bodyPart.hpp"
#include "capsule.hpp"
#include "revoluteJoint.hpp"

struct LimbBodyConfig : public BodyPartConfig {
  static LimbBodyConfig defaultConfig();
  CapsuleConfig templateCapsuleConfig;
  RevoluteJointConfig templateJointConfig;

  struct LimbSegmentConfig {
    float len;
    float radius;
  };
  std::vector<LimbSegmentConfig> segments;
  b2Vec2 basePos;
  b2Rot rotation;
};

class LimbBody : public BodyPart {
public:
  b2Vec2 getBasePos();
  b2Vec2 getEndPos();
  void aimAt(b2Vec2 worldPoint);
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

  friend class BodyFactory;
};
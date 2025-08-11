
#include "PIDScalarController.hpp"
#include "baseCreature.hpp"
#include "capsuleBodyPart.hpp"
#include "circleBodyPart.hpp"
#include "limbBodyPart.hpp"
#include <memory>
class Human : public BaseCreature {
public:
  struct HumanConfig {};

  std::shared_ptr<Human> create(HumanConfig config);
  static std::optional<HumanConfig>
  parseConfigFromJSON(const std::string &fileName);

protected:
  Human();
  std::shared_ptr<CapsuleBodyPart> torso;
  std::shared_ptr<CapsuleBodyPart> neck;
  std::shared_ptr<CircleBodyPart> leftShoulder;
  std::shared_ptr<CircleBodyPart> rightShoulder;
  std::shared_ptr<CircleBodyPart> leftHip;
  std::shared_ptr<CircleBodyPart> rightHip;

  std::shared_ptr<LimbBodyPart> leftLeg;
  std::shared_ptr<LimbBodyPart> rightLeg;
  std::shared_ptr<LimbBodyPart> leftArm;
  std::shared_ptr<LimbBodyPart> rightArm;

  PIDScalarController torsoRotationController;
  PIDScalarController torsoHeightController;

  b2JointId leftHipPrismaticJoint;
  b2JointId rightHipPrismaticJoint;
  b2JointId leftArmPrismaticJoint;
  b2JointId rightArmPrismaticJoint;
};
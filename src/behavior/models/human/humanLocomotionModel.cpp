#include "humanLocomotionModel.hpp"

void HumanLocomotionModel::update([[maybe_unused]] double dt) {
  updateJump(dt);
  updateLegs(dt);
}
HumanLocomotionModel::HumanLocomotionModel(HumanLocomotionModel::Config config,
                                           std::weak_ptr<LimbBody> leftLeg,
                                           std::weak_ptr<LimbBody> rightLeg,
                                           std::weak_ptr<Body> torso)
    : _config(config), _leftLeg(leftLeg), _rightLeg(rightLeg), _torso(torso) {}

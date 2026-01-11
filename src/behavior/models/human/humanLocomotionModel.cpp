#include "humanLocomotionModel.hpp"

void HumanLocomotionModel::update([[maybe_unused]] double dt) {
  updateJump(dt);
  updateLegs(dt);
}
HumanLocomotionModel::HumanLocomotionModel(HumanLocomotionModel::Config config)
    : _config(config), _leftLeg(config._bodyParts._leftLeg),
      _rightLeg(config._bodyParts._rightLeg), _torso(config._bodyParts._torso) {
}

void HumanLocomotionModel::updateJump([[maybe_unused]] double dt) {
  // TODO: implement
}

void HumanLocomotionModel::updateLegs([[maybe_unused]] double dt) {
  // TODO: implement
}

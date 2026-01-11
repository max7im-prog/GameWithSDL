#include "humanArticulationModel.hpp"

void HumanArticulationModel::update([[maybe_unused]] double dt) {
  updateAngleConstraints(dt);
  updateHipShoulderRotation(dt);
  updateNeckIncline(dt);
}
HumanArticulationModel::HumanArticulationModel(
    HumanArticulationModel::Config config)
    : _config(config), _leftLeg(config._bodyParts._leftLeg),
      _rightLeg(config._bodyParts._rightLeg), _torso(config._bodyParts._torso),
      _leftArm(config._bodyParts._leftArm),
      _rightArm(config._bodyParts._rightArm), _neck(config._bodyParts._neck),
      _head(config._bodyParts._head), _hips(config._bodyParts._hips),
      _shoulders(config._bodyParts._shoulders) {}

void updateAngleConstraints([[maybe_unused]] double dt) {
  // TODO: implement
}

void updateHipShoulderRotation([[maybe_unused]] double dt) {
  // TODO: implement
}

void updateNeckIncline([[maybe_unused]] double dt) {
  // TODO: implement
}

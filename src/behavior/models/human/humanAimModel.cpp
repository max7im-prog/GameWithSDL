#include "humanAimModel.hpp"

void HumanAimModel::update([[maybe_unused]] double dt) { updatePointAt(dt); }

HumanAimModel::HumanAimModel(HumanAimModel::Config config)
    : _config(config), _leftArm(config._bodyParts._leftArm),
      _rightArm(config._bodyParts._rightArm) {}

void HumanAimModel::updatePointAt([[maybe_unused]] double dt) {
  // TODO: implement
}

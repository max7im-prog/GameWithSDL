
#include "humanBalanceModel.hpp"

void HumanBalanceModel::update([[maybe_unused]] double dt) {

  updateTorsoGroundOffset(dt);
  updateTorsoTilt(dt);
  updateTorsoDamping(dt);
}

HumanBalanceModel::HumanBalanceModel(HumanBalanceModel::Config config)
    : _config(config), _torso(config._bodyParts._torso) {}

void HumanBalanceModel::updateTorsoGroundOffset([[maybe_unused]] double dt) {
  // TODO: implement
}

void HumanBalanceModel::updateTorsoTilt([[maybe_unused]] double dt) {
  // TODO: implement
}

void HumanBalanceModel::updateTorsoDamping([[maybe_unused]] double dt) {
  // TODO: implement
}

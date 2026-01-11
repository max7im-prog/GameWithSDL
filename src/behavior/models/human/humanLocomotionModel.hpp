#pragma once
#include "IBehaviorModel.hpp"
#include "limbBody.hpp"
#include <memory>
class HumanLocomotionModel : public IBehaviorModel {
public:
  struct Config {};
  virtual void update(double dt) override;
  HumanLocomotionModel(HumanLocomotionModel::Config config,
                       std::weak_ptr<LimbBody> leftLeg,
                       std::weak_ptr<LimbBody> rightLeg,
                       std::weak_ptr<Body> torso);

protected:
  void updateJump([[maybe_unused]] double dt);
  void updateLegs([[maybe_unused]] double dt);

  HumanLocomotionModel::Config _config;
  std::weak_ptr<LimbBody> _leftLeg;
  std::weak_ptr<LimbBody> _rightLeg;
  std::weak_ptr<Body> _torso;
};

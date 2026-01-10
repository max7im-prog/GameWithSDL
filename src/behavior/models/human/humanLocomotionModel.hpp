#pragma once
#include "IBehaviorModel.hpp"
#include "limbBody.hpp"
#include <memory>
class HumanLocomotionModel : public IBehaviorModel {
public:
  struct Config {};
  virtual void update(double secondsPassed) override;
  HumanLocomotionModel(HumanLocomotionModel::Config config,
                       std::weak_ptr<LimbBody> leftLeg,
                       std::weak_ptr<LimbBody> rightLeg,
                       std::weak_ptr<Body> torso);

protected:
  HumanLocomotionModel::Config _config;
  std::weak_ptr<LimbBody> _leftLeg;
  std::weak_ptr<LimbBody> _rightLeg;
  std::weak_ptr<Body> _torso;
};

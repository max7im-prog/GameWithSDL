#pragma once
#include "IBehaviorModel.hpp"
#include "limbBody.hpp"
#include <memory>
class HumanLocomotionModel : public IBehaviorModel {
public:
  struct Config {
    struct {
      std::weak_ptr<LimbBody> _leftLeg;
      std::weak_ptr<LimbBody> _rightLeg;
      std::weak_ptr<Body> _torso;
    } _bodyParts;
  };
  virtual void update(double dt) override;
  HumanLocomotionModel(HumanLocomotionModel::Config config);

protected:
  void updateJump([[maybe_unused]] double dt);
  void updateLegs([[maybe_unused]] double dt);

  HumanLocomotionModel::Config _config;
  std::weak_ptr<LimbBody> _leftLeg;
  std::weak_ptr<LimbBody> _rightLeg;
  std::weak_ptr<Body> _torso;
};

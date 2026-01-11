#pragma once
#include "IBehaviorModel.hpp"
#include "limbBody.hpp"
#include <memory>
class HumanAimModel : public IBehaviorModel {
public:
  struct Config {
    struct {
      std::weak_ptr<LimbBody> _leftArm;
      std::weak_ptr<LimbBody> _rightArm;
    } _bodyParts;
  };
  virtual void update(double dt) override;
  HumanAimModel(HumanAimModel::Config config);

protected:
  void updatePointAt([[maybe_unused]] double dt);

  HumanAimModel::Config _config;
  std::weak_ptr<LimbBody> _leftArm;
  std::weak_ptr<LimbBody> _rightArm;
};

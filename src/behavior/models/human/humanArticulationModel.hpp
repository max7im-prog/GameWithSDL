#pragma once
#include "IBehaviorModel.hpp"
#include "limbBody.hpp"
#include <memory>
class HumanArticulationModel : public IBehaviorModel {
public:
  struct Config {
    struct {
      std::weak_ptr<LimbBody> _leftLeg;
      std::weak_ptr<LimbBody> _rightLeg;
      std::weak_ptr<LimbBody> _leftArm;
      std::weak_ptr<LimbBody> _rightArm;
      std::weak_ptr<Body> _torso;
      std::weak_ptr<Body> _neck;
      std::weak_ptr<Body> _head;
      std::weak_ptr<GirdleConnection> _hips;
      std::weak_ptr<GirdleConnection> _shoulders;
    } _bodyParts;
  };
  virtual void update(double dt) override;
  HumanArticulationModel(HumanArticulationModel::Config config);

protected:
  void updateAngleConstraints([[maybe_unused]] double dt);
  void updateHipShoulderRotation([[maybe_unused]] double dt);
  void updateNeckIncline([[maybe_unused]] double dt);

  HumanArticulationModel::Config _config;
  std::weak_ptr<LimbBody> _leftLeg;
  std::weak_ptr<LimbBody> _rightLeg;
  std::weak_ptr<LimbBody> _leftArm;
  std::weak_ptr<LimbBody> _rightArm;
  std::weak_ptr<Body> _torso;
  std::weak_ptr<Body> _neck;
  std::weak_ptr<Body> _head;
  std::weak_ptr<GirdleConnection> _hips;
  std::weak_ptr<GirdleConnection> _shoulders;
};

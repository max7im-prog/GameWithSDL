
#pragma once
#include "IBehaviorModel.hpp"
#include "body.hpp"
#include <memory>
class HumanBalanceModel : public IBehaviorModel {
public:
  struct Config {
    struct {
      std::weak_ptr<Body> _torso;
    } _bodyParts;
  };
  virtual void update(double dt) override;
  HumanBalanceModel(HumanBalanceModel::Config config);

protected:
  void updateTorsoGroundOffset([[maybe_unused]] double dt);
  void updateTorsoTilt([[maybe_unused]] double dt);
  void updateTorsoDamping([[maybe_unused]] double dt);

  HumanBalanceModel::Config _config;
  std::weak_ptr<Body> _torso;
};

#pragma once

class IBehaviorModel {
public:
  virtual void update(double secondsPassed) = 0;
  virtual ~IBehaviorModel() = 0;
};

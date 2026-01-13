#pragma once

class IBehaviorModel {
public:
  virtual void update([[maybe_unused]] double secondsPassed) = 0;
  virtual ~IBehaviorModel() = 0;

protected:
  IBehaviorModel() = default;

private:
  IBehaviorModel(const IBehaviorModel &other) = delete;
  IBehaviorModel(const IBehaviorModel &&other) = delete;
  IBehaviorModel operator=(const IBehaviorModel &other) = delete;
  IBehaviorModel operator=(const IBehaviorModel &&other) = delete;
};

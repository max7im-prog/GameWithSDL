#pragma once
#include <box2d/box2d.h>
#include <cmath>

struct PIDVectorControllerConfig {
  float kp = 0;
  float kd = 0;
  float ki = 0;
  float maxForce = INFINITY;
  b2Vec2 constant = b2Vec2(0, 0);
};

// Class to perform proportional derivative control
// (https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)
class PIDVectorController {
public:
  PIDVectorController(const PIDVectorControllerConfig &config);
  PIDVectorController();
  ~PIDVectorController() = default;

  // Updates a state of a PID controller and returns the new output of a
  // controller Error in this context is DESIRED value minus CURRENT value
  b2Vec2 update(b2Vec2 error, float dt);

  void reset();

private:
  float kp, ki, kd, maxForce;
  b2Vec2 integral;
  b2Vec2 previousError;
  b2Vec2 constant;
};
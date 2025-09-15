#pragma once
#include "box2d/math_functions.h"
#include <cmath>

struct PIDRotControllerConfig {
  float kp = 0;
  float ki = 0;
  float kd = 0;
  float maxTorque = INFINITY;
  float constant = 0; 
};

// Class to perform proportional derivative control
// (https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)
class PIDRotController {
public:
  PIDRotController(const PIDRotControllerConfig &config);
  PIDRotController();
  ~PIDRotController() = default;

  // Updates the PID state and returns the correction as a rotation increment.
  // `error` is the desired rotation relative to the current one (target *
  // inv(current)).
  b2Rot update(b2Rot error, float dt);

  void reset();

private:
  float kp, ki, kd, maxTorque;
  float constant;

  float integral;      // accumulated angle error
  float previousError; // last frame’s angle error
};
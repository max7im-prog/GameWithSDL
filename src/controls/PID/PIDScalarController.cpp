#include "PIDScalarController.hpp"

PIDScalarController::PIDScalarController(
    const PIDScalarControllerConfig &config)
    : kp(config.kp), ki(config.ki), kd(config.kd), maxForce(config.maxForce),
      integral(0.0f), previousError(0.0f), constant(config.constant) {}

PIDScalarController::PIDScalarController()
    : kp(0), ki(0), kd(0), maxForce(INFINITY), integral(0), previousError(0) {}

float PIDScalarController::update(float error, float dt) {
  integral += error * dt;
  float derivative = error - previousError;
  previousError = error;
  float output = constant + kp * error + ki * integral + kd * derivative;

  if (integral > maxForce) {
    integral = maxForce;
  } else if (integral < -maxForce) {
    integral = -maxForce;
  }

  if (output > maxForce) {
    output = maxForce;
  } else if (output < -maxForce) {
    output = -maxForce;
  }

  return output;
}

void PIDScalarController::reset() {
  integral = 0.0f;
  previousError = 0.0f;
}

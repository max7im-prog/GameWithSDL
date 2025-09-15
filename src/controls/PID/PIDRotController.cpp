
#include "PIDRotController.hpp"

PIDRotController::PIDRotController(const PIDRotControllerConfig &config)
    : kp(config.kp), ki(config.ki), kd(config.kd), maxTorque(config.maxTorque),
      constant(config.constant), integral(0.0f), previousError(0.0f) {}

PIDRotController::PIDRotController()
    : kp(0.0f), ki(0.0f), kd(0.0f), maxTorque(INFINITY), constant(0.0f),
      integral(0.0f), previousError(0.0f) {}

b2Rot PIDRotController::update(b2Rot error, float dt) {
  // Convert rotation error to scalar angle
  float angleError = b2Rot_GetAngle(error);

  // PID terms
  integral += angleError * dt;
  float derivative = (angleError - previousError) / dt;
  previousError = angleError;

  float output = constant + (kp * angleError + ki * integral + kd * derivative);

  // Clamp output
  if (std::abs(integral) > maxTorque) {
    integral = (integral > 0 ? 1 : -1) * maxTorque;
  }
  if (std::abs(output) > maxTorque) {
    output = (output > 0 ? 1 : -1) * maxTorque;
  }

  // Return as a small corrective rotation
  return b2MakeRot(output);
}

void PIDRotController::reset() {
  integral = 0.0f;
  previousError = 0.0f;
}
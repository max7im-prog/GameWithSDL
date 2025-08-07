#include "PIDVectorController.hpp"

PIDVectorController::PIDVectorController(
    const PIDVectorControllerConfig &config)
    : kp(config.kp), ki(config.ki), kd(config.kd), maxForce(config.maxForce),
      integral({0, 0}), previousError({0, 0}), constant(config.constant) {}

PIDVectorController::PIDVectorController()
    : kp(0), ki(0), kd(0), maxForce(INFINITY), integral({0, 0}),
      previousError({0, 0}), constant(b2Vec2(0, 0)) {}

b2Vec2 PIDVectorController::update(b2Vec2 error, float dt) {
  integral = b2Add(integral, b2MulSV(dt, error));
  b2Vec2 derivative = b2Sub(error, previousError);
  previousError = error;
  b2Vec2 output =
      b2Add(constant, b2Add(b2Add(b2MulSV(kp, error), b2MulSV(ki, integral)),
                            b2MulSV(kd, derivative)));

  if (b2Length(integral) > maxForce) {
    integral = b2MulSV(maxForce, b2Normalize(integral));
  }

  if (b2Length(output) > maxForce) {
    output = b2MulSV(maxForce, b2Normalize(output));
  }

  return output;
}

void PIDVectorController::reset() {
  this->integral = {0, 0};
  this->previousError = {0, 0};
}
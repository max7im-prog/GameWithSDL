#include "PIDScalarController.hpp"

PIDScalarController::PIDScalarController(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd), integral(0.0f), previousError(0.0f)
{
}

PIDScalarController::~PIDScalarController()
{
}

float PIDScalarController::update(float error, float dt)
{
    integral+= error*dt;
    float derivative = error-previousError;
    previousError = error;
    float output = kp*error + ki*integral+kd*derivative;
    return output;
}

void PIDScalarController::reset()
{
    integral = 0.0f;
    previousError = 0.0f;
}

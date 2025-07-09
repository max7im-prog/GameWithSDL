#include "PIDController.hpp"

PIDController::PIDController(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd), integral(0.0f), previousError(0.0f)
{
}

PIDController::~PIDController()
{
}

float PIDController::update(float error, float dt)
{
    integral+= error*dt;
    float derivative = error-previousError;
    previousError = error;
    float output = kp*error + ki*integral+kd*derivative;
    return output;
}

void PIDController::reset()
{
    integral = 0.0f;
    previousError = 0.0f;
}

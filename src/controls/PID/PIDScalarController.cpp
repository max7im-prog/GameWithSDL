#include "PIDScalarController.hpp"

PIDScalarController::PIDScalarController(float kp, float ki, float kd, float maxForce) : kp(kp), ki(ki), kd(kd), maxForce(maxForce), integral(0.0f), previousError(0.0f)
{
}

PIDScalarController::~PIDScalarController()
{
}

PIDScalarController::PIDScalarController():kp(0), ki(0), kd(0), maxForce(INFINITY), integral(0), previousError(0)
{
}

float PIDScalarController::update(float error, float dt)
{
    integral += error * dt;
    float derivative = error - previousError;
    previousError = error;
    float output = kp * error + ki * integral + kd * derivative;

    if (output > maxForce)
    {
        output = maxForce;
    }
    else if (output < -maxForce)
    {
        output = -maxForce;
    }

    return output;
}

void PIDScalarController::reset()
{
    integral = 0.0f;
    previousError = 0.0f;
}

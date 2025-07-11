#pragma once
#include <cmath>

// Class to perform proportional derivative control (https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)
class PIDScalarController{
public:
    PIDScalarController(float kp, float ki, float kd,float maxForce = INFINITY);
    ~PIDScalarController();
    PIDScalarController();

    // Updates a state of a PID controller and returns the new output of a controller
    // Error in this context is DESIRED value minus CURRENT value
    float update(float error, float dt);

    void reset();
private:
    float kp, ki, kd, maxForce;
    float integral;
    float previousError;
};
#pragma once

// Class to perform proportional derivative control (https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)
class PIDController{
public:
    PIDController(float kp, float ki, float kd);
    ~PIDController();

    // Updates a state of a PID controller and returns the new output of a controller
    float update(float error, float dt);

    void reset();
private:
    float kp, ki, kd;
    float integral;
    float previousError;
};
#pragma once
#include <box2d/box2d.h>
#include <cmath>

// Class to perform proportional derivative control (https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)
class PIDVectorController{
public:
    PIDVectorController(float kp, float ki, float kd,float maxForce = INFINITY);
    PIDVectorController();
    ~PIDVectorController();

    // Updates a state of a PID controller and returns the new output of a controller
    b2Vec2 update(b2Vec2 error, float dt);

    void reset();
private:
    float kp, ki, kd, maxForce;
    b2Vec2 integral;
    b2Vec2 previousError;
};
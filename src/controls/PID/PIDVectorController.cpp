#include "PIDVectorController.hpp"

PIDVectorController::PIDVectorController(float kp, float ki, float kd): kp(kp), ki(ki), kd(kd), integral({0,0}), previousError({0,0})
{
}

PIDVectorController::~PIDVectorController()
{
}

b2Vec2 PIDVectorController::update(b2Vec2 error, float dt)
{
    integral = b2Add(integral,b2MulSV(dt,error));
    b2Vec2 derivative = b2Sub(error,previousError);
    previousError = error;
    b2Vec2 output = b2Add(b2Add(b2MulSV(kp,error),b2MulSV(ki,integral)),b2MulSV(kd,derivative));
    return output;
}

void PIDVectorController::reset()
{
    this->integral = {0,0};
    this->previousError = {0,0};
}
#pragma once
#include <entt/entt.hpp>

class MouseJointSystem{
private:
    bool buttonPressed;

public:
    MouseJointSystem();
    ~MouseJointSystem();

    void update(entt::registry& registry);

};
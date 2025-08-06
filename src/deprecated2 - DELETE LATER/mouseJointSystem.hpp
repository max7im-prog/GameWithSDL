#pragma once
#include <entt/entt.hpp>
#include "renderContext.hpp"

class MouseJointSystem{
private:
    bool buttonPressed;
    bool jointExists;
    entt::entity mouseBody;
    entt::entity mouseJoint;
public:
    MouseJointSystem(entt::registry& registry);
    MouseJointSystem();
    ~MouseJointSystem();

    void update(entt::registry& registry, const RenderContext &renderContext);

};
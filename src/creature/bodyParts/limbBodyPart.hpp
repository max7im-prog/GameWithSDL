#pragma once
#include "bodyPart.hpp"
#include <optional>
#include "PIDController.hpp"

class LimbBodyPart : public BodyPart
{
public:
    LimbBodyPart(entt::registry &registry,
                 b2WorldId worldId,
                 b2Vec2 worldPoint1,
                 b2Vec2 worldPoint2,
                 std::vector<std::pair<float, float>> portionRadiusPairs,
                 std::optional<b2Filter> shapeFilter = std::nullopt);
    ~LimbBodyPart();

    // Returns a position of a base of a limb
    b2Vec2 getBase();

    // Returns a position of an end of a limb
    b2Vec2 getEnd();

    // Returns positions of joints in a limb, including the beginning and the end of a limb
    std::vector<b2Vec2> getJointsPos();

    void trackPoint(b2Vec2 worldPoint, bool track);

    std::pair<entt::entity, b2JointId> getConnectionJoint();
    std::pair<entt::entity, b2JointId> connect(b2BodyId bodyId, b2Vec2 worldPoint);

    void update(float dt) override;

protected:

    // Stores PID controllers for each segment of a limb (2 controllers for each segment - 1 for beginning and 1 for end of a segment)
    std::vector<std::pair<PIDController,PIDController>> PIDControllers;
    std::pair<entt::entity, b2JointId> connectionJoint = {entt::null, b2JointId{}};
    b2Vec2 trackingPoint;
    bool isTracking;

    // Points a limb at a world point by using inverse kinematics and applying appropriate forces to segments of a limb
    void updateTracking(float dt);
};
#include "debugUtils.hpp"

#include <iostream>

#include "physicsComponents.hpp"
#include "renderComponents.hpp"
#include "eventComponents.hpp"
#include "eventComponents.hpp"

void DebugUtils::debugPrintRegistry(entt::registry &registry)
{
    std::cout << "================= REGISTRY DUMP =================\n";

    for(auto entity: registry.view<entt::entity>())  {
        std::cout << "Entity " << static_cast<uint32_t>(entity) << ":\n";

        if (registry.all_of<QuitEvent>(entity)) {
            std::cout << "  - QuitEvent\n";
        }

        if (registry.all_of<PlayerInput>(entity)) {
            std::cout << "  - PlayerInput\n";
        }

        if (registry.all_of<KeyPress>(entity)) {
            const auto &kp = registry.get<KeyPress>(entity);
            std::cout << "  - KeyPress: key=" << SDL_GetKeyName(kp.key)
                      << ", pressed=" << (kp.pressed ? "true" : "false") << "\n";
        }

        if (registry.all_of<ButtonPress>(entity)) {
            const auto &bp = registry.get<ButtonPress>(entity);
            std::cout << "  - ButtonPress: button=" << static_cast<int>(bp.button)
                      << ", pressed=" << (bp.pressed ? "true" : "false")
                      << ", location=(" << bp.location.first << ", " << bp.location.second << ")\n";
        }

        if (registry.all_of<Mouse>(entity)) {
            std::cout << "  - Mouse\n";
        }

        if (registry.all_of<PhysicsWorld>(entity)) {
            const auto &pw = registry.get<PhysicsWorld>(entity);
            std::cout << "  - PhysicsWorld: worldId=" << pw.worldId.index1 << "\n";
        }

        if (registry.all_of<PhysicsBody>(entity)) {
            const auto &pb = registry.get<PhysicsBody>(entity);
            std::cout << "  - PhysicsBody: bodyId=" << pb.bodyId.index1
                      << ", worldId=" << pb.worldId.index1
                      << ", shapes=" << pb.shapes.size() << "\n";
        }

        if (registry.all_of<PhysicsJoint>(entity)) {
            const auto &pj = registry.get<PhysicsJoint>(entity);
            std::cout << "  - PhysicsJoint: jointId=" << pj.jointId.index1
                      << ", bodyAId=" << pj.bodyAId.index1
                      << ", bodyBId=" << pj.bodyBId.index1
                      << ", worldId=" << pj.worldId.index1 << "\n";
        }

        if (registry.all_of<PendingPhysicsBody>(entity)) {
            const auto &ppb = registry.get<PendingPhysicsBody>(entity);
            std::cout << "  - PendingPhysicsBody: worldId=" << ppb.worldId.index1
                      << ", shapeDefs=" << ppb.shapeDefs.size() << "\n";
        }

        if (registry.all_of<texture>(entity)) {
            const auto &tex = registry.get<texture>(entity);
            std::cout << "  - texture: " << (tex.texture.has_value() ? "set" : "none") << "\n";
        }
    }

    std::cout << "=================================================\n";
}


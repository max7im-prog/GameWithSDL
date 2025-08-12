#include "debugUtils.hpp"

#include <iostream>

#include "physicsComponents.hpp"
#include "renderComponents.hpp"
#include "eventComponents.hpp"
#include "eventComponents.hpp"

void DebugUtils::debugPrintRegistry(entt::registry &registry)
{
    // std::cout << "================= REGISTRY DUMP =================\n";

    // for(auto entity: registry.view<entt::entity>())  {
    //     std::cout << "Entity " << static_cast<uint32_t>(entity) << ":\n";

    //     if (registry.all_of<QuitEvent>(entity)) {
    //         std::cout << "  - QuitEvent\n";
    //     }

    //     if (registry.all_of<PlayerInput>(entity)) {
    //         std::cout << "  - PlayerInput\n";
    //     }

    //     if (registry.all_of<KeyPressEvent>(entity)) {
    //         const auto &kp = registry.get<KeyPressEvent>(entity);
    //         std::cout << "  - KeyPress: key=" << SDL_GetKeyName(kp.key)
    //                   << ", pressed=" << (kp.pressed ? "true" : "false") << "\n";
    //     }

    //     if (registry.all_of<ButtonPressEvent>(entity)) {
    //         const auto &bp = registry.get<ButtonPressEvent>(entity);
    //         std::cout << "  - ButtonPress: button=" << static_cast<int>(bp.button)
    //                   << ", pressed=" << (bp.pressed ? "true" : "false")
    //                   << ", location=(" << bp.location.first << ", " << bp.location.second << ")\n";
    //     }

    //     if (registry.all_of<Mouse>(entity)) {
    //         std::cout << "  - Mouse\n";
    //     }

    //     // TODO: create output for all other components

        

    //     if (registry.all_of<texture>(entity)) {
    //         const auto &tex = registry.get<texture>(entity);
    //         std::cout << "  - texture: " << (tex.texture.has_value() ? "set" : "none") << "\n";
    //     }
    // }

    // std::cout << "=================================================\n";
}


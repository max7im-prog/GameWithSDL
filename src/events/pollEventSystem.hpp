#ifndef POLLEVENTSYSTEM_HPP
#define POLLEVENTSYSTEM_HPP

#include <entt/entt.hpp>

// System to poll SDL events (mostly player's input)
class PollEventSystem{
public:
    PollEventSystem();
    ~PollEventSystem();

    void update(entt::registry& registry);
};

#endif //POLLEVENTSYSTEM_HPP
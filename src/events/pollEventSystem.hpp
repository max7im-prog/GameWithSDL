#ifndef POLLEVENTSYSTEM_HPP
#define POLLEVENTSYSTEM_HPP

#include <entt/entt.hpp>

class PollEventSystem{
public:
    PollEventSystem();
    ~PollEventSystem();

    void update(entt::registry& registry);
};

#endif //POLLEVENTSYSTEM_HPP
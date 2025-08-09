#pragma once

#ifndef QUITSYSTEM_HPP
#define QUITSYSTEM_HPP

#include <entt/entt.hpp>


class Game;

// System to handle exiting the game
class QuitSystem{
public:
    QuitSystem();
    ~QuitSystem();

    void update(entt::registry &registry,bool& running);
};



#endif //QUITSYSTEM_HPP
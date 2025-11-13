#pragma once

#include "gameSystem.hpp"
#ifndef QUITSYSTEM_HPP
#define QUITSYSTEM_HPP

#include <entt/entt.hpp>


class Game;

// System to handle exiting the game
class QuitSystem:public GameSystem{
public:
    QuitSystem();
    ~QuitSystem();

    void update(entt::registry &registry,bool& running, double secondsPassed);
};



#endif //QUITSYSTEM_HPP
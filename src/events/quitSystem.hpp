#pragma once

#ifndef QUITSYSTEM_HPP
#define QUITSYSTEM_HPP

#include <entt/entt.hpp>


class Game;

class QuitSystem{
public:
    QuitSystem();
    ~QuitSystem();

    void update(entt::registry &registry,Game& game);
};



#endif //QUITSYSTEM_HPP
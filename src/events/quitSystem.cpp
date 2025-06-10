#include "quitSystem.hpp"
#include "events.hpp"
#include "game.hpp"
QuitSystem::QuitSystem()
{
}

QuitSystem::~QuitSystem()
{
}

void QuitSystem::update(entt::registry &registry, Game &game)
{
    auto view = registry.view<QuitEvent>();
    for(auto entity:view){
        game.setRunning(false);
        registry.destroy(entity);
    }
}

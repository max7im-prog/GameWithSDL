#include "quitSystem.hpp"
#include "eventComponents.hpp"
QuitSystem::QuitSystem()
{
}

QuitSystem::~QuitSystem()
{
}

void QuitSystem::update(entt::registry &registry,bool& running)
{
    auto view = registry.view<QuitEvent>();
    for(auto entity:view){
        running = false;
        registry.destroy(entity);
    }
}

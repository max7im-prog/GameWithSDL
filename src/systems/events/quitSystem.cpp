#include "quitSystem.hpp"
#include "eventComponents.hpp"
#include "gameSystem.hpp"
QuitSystem::QuitSystem():GameSystem("QuitSystem")
{
}

QuitSystem::~QuitSystem()
{
}

void QuitSystem::update(entt::registry &registry,bool& running, double secondsPassed)
{
  if(!shouldRun(secondsPassed)){
    return;
  }
    auto view = registry.view<QuitEvent>();
    for(auto entity:view){
        running = false;
        registry.destroy(entity);
    }
}

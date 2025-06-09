// main.cpp

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

int main(int argc, char* argv[]) {
    // SDL3 basic init/quit
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Log("SDL initialized successfully.");

    // Box2D: create a world with gravity
    b2Vec2 gravity({2,2});
    // b2World world(gravity);
    SDL_Log("box2d initialized successfully.");

    // EnTT: create a registry and an entity
    entt::registry registry;
    auto entity = registry.create();

    SDL_Log("EnTT initialized successfully.");

    SDL_Quit();
    return 0;
}

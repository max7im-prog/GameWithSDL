
#include "structure.hpp"

Structure::~Structure() = default;

Structure::Structure(entt::registry &registry) : RegistryComposite(registry) {}

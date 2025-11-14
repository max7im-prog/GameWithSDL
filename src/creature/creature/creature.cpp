#include "creature.hpp"
#include "body.hpp"
#include "box2d/types.h"
#include "demoCreature.hpp"
#include "physicsUtils.hpp"
#include "registryComposite.hpp"
#include <stdexcept>

std::uint32_t Creature::getAbilities() { return creatureAbilities; }

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : SceneNode(registry), world(world) {}

Creature::~Creature() {}

void Creature::update(float dt) { RegistryComposite::update(dt); }

b2Filter CreatureConfig::defaultFilter() {
  b2Filter ret = b2DefaultFilter();
  ret.categoryBits = ObjectCategory::CREATURE;
  return ret;
};

void Creature::registerBody(std::weak_ptr<Body> body, const std::string &name) {
  if (_bodies.find(name) != _bodies.end()) {
    throw std::runtime_error("Body with name " + name +
                             " registered more than once");
  }
  _bodies[name] = body;
  registerChild(body);
}

void Creature::registerConnection(std::weak_ptr<Connection> connection,
                                  const std::string &name) {
  if (_connections.find(name) != _connections.end()) {
    throw std::runtime_error("Connection with name " + name +
                             " registered more than once");
  }
  _connections[name] = connection;
  registerChild(connection);
}

const std::unordered_map<std::string, std::weak_ptr<Body>> &
Creature::getBodies() const {
  return _bodies;
}
const std::unordered_map<std::string, std::weak_ptr<Connection>> &
Creature::getConnections() const {
  return _connections;
}
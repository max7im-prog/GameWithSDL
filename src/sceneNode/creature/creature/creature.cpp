#include "creature.hpp"

#include "body.hpp"
#include "box2d/types.h"
#include "connection.hpp"
#include "physicsUtils.hpp"
#include "registryComposite.hpp"

#include <stdexcept>

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : SceneNode(registry), world(world) {}

Creature::~Creature() {}

void Creature::update(float dt) {
  for (auto &behavior : _behaviors) {
    behavior->update(dt);
  }
  SceneNode::update(dt);
}

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

void Creature::perform(CreatureAction action, InputState inputState) {
  if (auto it = _actions.find(action); it != _actions.end()) {
    it->second(inputState);
  }
}

void Creature::registerBehavior(std::shared_ptr<IBehaviorModel> behavior) {
  _behaviors.push_back(behavior);
}

void Creature::clearBehaviors() { _behaviors.clear(); }

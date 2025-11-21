#include "sceneNode.hpp"
#include "registryComposite.hpp"

SceneNode::~SceneNode() = default;

SceneNode::SceneNode(entt::registry &registry) : RegistryComposite(registry) {}

const std::unordered_map<std::string, std::weak_ptr<Body>> &
SceneNode::getBodies() const {
  return _bodies;
}

const std::unordered_map<std::string, std::weak_ptr<Connection>> &
SceneNode::getConnections() const {
  return _connections;
}
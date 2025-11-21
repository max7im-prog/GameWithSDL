#include "sceneNode.hpp"
#include "registryComposite.hpp"

SceneNode::~SceneNode() = default;

SceneNode::SceneNode(entt::registry &registry) : RegistryComposite(registry) {}
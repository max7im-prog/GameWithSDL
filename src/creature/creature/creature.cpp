#include "creature.hpp"

std::uint32_t Creature::getAbilities() { return abilitiesFlags; }

Creature::Creature(entt::registry &registry, const std::shared_ptr<World> world)
    : registry(registry), world(world) {}

void Creature::registerBodyPart(std::shared_ptr<BodyPart> bodyPart) {
  bodyParts.push_back(bodyPart);
}

void Creature::registerJoint(std::shared_ptr<Joint> joint) {
  joints.push_back(joint);
}

void Creature::remove() {
  if (registry.valid(entity)) {
    registry.destroy(entity);
  }
  entity = entt::null;
}

Creature::~Creature() {
  remove();

  for (auto j : joints) {
    j->remove();
  }

  for (auto bp : bodyParts) {
    bp->remove();
  }
}

void Creature::update(float dt){
  for(auto bp:bodyParts){
    bp->update(dt);
  }
}
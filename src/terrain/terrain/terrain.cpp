#include "terrain.hpp"
#include "physicsUtils.hpp"
#include "registryObject.hpp"

void Terrain::update(float dt) { RegistryObject::update(dt); }

Terrain::~Terrain() {}

Terrain::Terrain(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryObject(registry), world(world) {}

b2Filter TerrainConfig::defaultFilter() {
  b2Filter ret = b2DefaultFilter();
  ret.categoryBits = ObjectCategory::TERRAIN;
  return ret;
};
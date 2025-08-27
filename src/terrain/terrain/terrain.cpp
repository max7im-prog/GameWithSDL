#include "terrain.hpp"
#include "physicsUtils.hpp"
#include "registryComposite.hpp"

void Terrain::update(float dt) { RegistryComposite::update(dt); }

Terrain::~Terrain() {}

Terrain::Terrain(entt::registry &registry, const std::shared_ptr<World> world)
    : RegistryComposite(registry), world(world) {}

b2Filter TerrainConfig::defaultFilter() {
  b2Filter ret = b2DefaultFilter();
  ret.categoryBits = ObjectCategory::TERRAIN;
  return ret;
};
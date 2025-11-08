#include "terrain.hpp"
#include "box2d/types.h"
#include "jsonUtils.hpp"
#include "physicsUtils.hpp"
#include "registryComposite.hpp"
#include <string>

void Terrain::update(float dt) { RegistryComposite::update(dt); }

Terrain::~Terrain() {}

Terrain::Terrain(entt::registry &registry, const std::shared_ptr<World> world)
    : SceneNode(registry), world(world) {}

b2Filter TerrainConfig::defaultFilter() {
  b2Filter ret = b2DefaultFilter();
  ret.categoryBits = ObjectCategory::TERRAIN;
  return ret;
};

TerrainConfig::BodyParams
TerrainConfig::parseBodyParams(const nlohmann::json &json) {
  TerrainConfig::BodyParams ret{};

  ret._bodyDef = b2DefaultBodyDef();
  if (json.contains("body")) {
    auto &bodyJson = json["body"];
    {
      auto bodyType = JsonUtils::getOptional<std::string>(bodyJson, "type");
      if (bodyType) {
        if (*bodyType == "static") {
          ret._bodyDef.type = b2_staticBody;
        } else if (*bodyType == "dynamic") {
          ret._bodyDef.type = b2_dynamicBody;
        } else if (*bodyType == "kinematic") {
          ret._bodyDef.type = b2_kinematicBody;
        } else {
          // TODO: log unknown body type
        }
      }
    }
    ret._bodyDef.linearDamping = JsonUtils::getOrDefault<float>(
        bodyJson, "linearDamping", ret._bodyDef.linearDamping);
    ret._bodyDef.angularDamping = JsonUtils::getOrDefault<float>(
        bodyJson, "angularDamping", ret._bodyDef.angularDamping);
    ret._bodyDef.fixedRotation = JsonUtils::getOrDefault<bool>(
        bodyJson, "fixedRotation", ret._bodyDef.fixedRotation);
  }

  ret._shapeDef = b2DefaultShapeDef();
  if (json.contains("shape")) {
    auto &shapeJson = json["shape"];
    if (shapeJson.contains("material")) {
      auto &materialJson = shapeJson["material"];
      ret._shapeDef.material.friction = JsonUtils::getOrDefault<float>(
          materialJson, "friction", ret._shapeDef.material.friction);
      ret._shapeDef.material.restitution = JsonUtils::getOrDefault<float>(
          materialJson, "restitution", ret._shapeDef.material.restitution);
      ret._shapeDef.material.rollingResistance =
          JsonUtils::getOrDefault<float>(materialJson, "rollingResistance",
                                  ret._shapeDef.material.rollingResistance);
    }
    ret._shapeDef.density =
        JsonUtils::getOrDefault<float>(shapeJson, "density", ret._shapeDef.density);
  }

  return ret;
}

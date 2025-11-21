#include "sceneNode.hpp"
#include "jsonUtils.hpp"
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

SceneNodeConfig::~SceneNodeConfig() = default;

SceneNodeConfig::Transform
SceneNodeConfig::parseObjectTransform(const nlohmann::json &objectJson,
                                      const nlohmann::json &roomJson) {
  SceneNodeConfig::Transform ret{};

  // Position
  float roomX = JsonUtils::getOrDefault<float>(roomJson, "x", 0.0f);
  float roomY = JsonUtils::getOrDefault<float>(roomJson, "y", 0.0f);
  float offsetX = JsonUtils::getOrDefault<float>(objectJson, "x", 0.0f);
  float offsetY = JsonUtils::getOrDefault<float>(objectJson, "y", 0.0f);
  ret._originPos = {roomX, roomY};
  ret._relativePos = {offsetX, offsetY};

  // Rotation
  float rotationAngle =
      JsonUtils::getOrDefault<float>(objectJson, "rotation", 0);
  float rootAngle = 0;
  ret._relativeRot = b2MakeRot((B2_PI * 2) * rotationAngle / 360.0f);
  ret._rootRot = b2MakeRot((B2_PI * 2) * rootAngle / 360.0f);

  // Scale
  ret._scaleX = JsonUtils::getOrDefault<float>(objectJson, "scaleX", 1.0f);
  ret._scaleY = JsonUtils::getOrDefault<float>(objectJson, "scaleY", 1.0f);

  // Flip
  ret._flipX = JsonUtils::getOrDefault<bool>(objectJson, "flipX", false);
  ret._flipY = JsonUtils::getOrDefault<bool>(objectJson, "flipY", false);

  return ret;
}

std::shared_ptr<SceneNodeRenderConfig>
SceneNodeConfig::parseRenderConfig(const nlohmann::json &json) {
  using Config = SceneNodeRenderConfig;
  using BodyConfig = SceneNodeRenderConfig::BodyRenderConfig;
  using ShapeConfig =
      SceneNodeRenderConfig::BodyRenderConfig::ShapeRenderConfig;

  auto cfg = std::make_shared<Config>();

  //
  // 1. Top-level render sequence
  //
  if (json.contains("renderSequence") && json["renderSequence"].is_array()) {
    for (auto &item : json["renderSequence"]) {
      if (item.is_string()) {
        cfg->_renderSequence.push_back(item.get<std::string>());
      }
    }
  }

  //
  // 2. bodyRenders
  //
  if (json.contains("bodyRenders") && json["bodyRenders"].is_object()) {

    for (auto &[bodyName, bodyJson] : json["bodyRenders"].items()) {
      if (!bodyJson.is_object())
        continue;

      auto bodyCfg = std::make_shared<BodyConfig>();

      //
      // 2.1 Body render sequence
      //
      if (bodyJson.contains("renderSequence") &&
          bodyJson["renderSequence"].is_array()) {

        for (auto &shapeName : bodyJson["renderSequence"]) {
          if (shapeName.is_string()) {
            bodyCfg->_renderSequence.push_back(shapeName.get<std::string>());
          }
        }
      }

      //
      // 2.2 Shape renders
      //
      if (bodyJson.contains("shapeRenders") &&
          bodyJson["shapeRenders"].is_object()) {

        for (auto &[shapeName, shapeJson] : bodyJson["shapeRenders"].items()) {
          if (!shapeJson.is_object())
            continue;

          auto shapeCfg = std::make_shared<ShapeConfig>();

          //
          // 2.2.1 Textures list
          //
          if (shapeJson.contains("textures") &&
              shapeJson["textures"].is_array()) {
            for (auto &tex : shapeJson["textures"]) {
              if (tex.is_string()) {
                shapeCfg->_textures.push_back(tex.get<std::string>());
              }
            }
          }

          bodyCfg->_shapeRenders[shapeName] = shapeCfg;
        }
      }

      cfg->_bodyRenders[bodyName] = bodyCfg;
    }
  }

  return cfg;
}
#include "sceneNode.hpp"
#include "jsonUtils.hpp"
#include "registryComposite.hpp"
#include <iostream>

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

  float roomX{0};
  float roomY{0};
  float offsetX{0};
  float offsetY{0};

  if (roomJson.contains("pos")) {
    roomX = JsonUtils::getOrDefault<float>(roomJson["pos"], "x", 0.0f);
    roomY = JsonUtils::getOrDefault<float>(roomJson["pos"], "y", 0.0f);
  } else {
    // TODO: log error
  }

  if (objectJson.contains("pos")) {
    offsetX = JsonUtils::getOrDefault<float>(objectJson["pos"], "x", 0.0f);

    offsetY = JsonUtils::getOrDefault<float>(objectJson["pos"], "y", 0.0f);
  } else {
    // TODO: log error
  }
  ret._originPos = {roomX, roomY};
  ret._relativePos = {offsetX, offsetY};

  if (objectJson.contains("transform")) {
    // Rotation
    float rotationAngle =
        JsonUtils::getOrDefault<float>(objectJson["transform"], "rotation", 0);
    float rootAngle = 0;
    ret._relativeRot = b2MakeRot((B2_PI * 2) * rotationAngle / 360.0f);
    ret._rootRot = b2MakeRot((B2_PI * 2) * rootAngle / 360.0f);

    // Scale
    ret._scaleX =
        JsonUtils::getOrDefault<float>(objectJson["transform"], "scaleX", 1.0f);
    ret._scaleY =
        JsonUtils::getOrDefault<float>(objectJson["transform"], "scaleY", 1.0f);

    // Flip
    ret._flipX =
        JsonUtils::getOrDefault<bool>(objectJson["transform"], "flipX", false);
    ret._flipY =
        JsonUtils::getOrDefault<bool>(objectJson["transform"], "flipY", false);
  } else {
    // TODO: log error
  }

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
          {
            auto texture =
                JsonUtils::getOptional<std::string>(shapeJson, "texture");
            if (texture) {
              shapeCfg->_texture = *texture;
            }
          }

          {
            auto numTextures =
                JsonUtils::getOptional<int>(shapeJson, "numTextures");
            if (numTextures) {
              shapeCfg->_numTextures = *numTextures;
            }
          }
          {
            shapeCfg->_offsetPerTexture = {0, 0};
            // shapeCfg->_initialRect = ;
          }
          bodyCfg->_shapeRenders[shapeName] = shapeCfg;
        }
      }

      cfg->_bodyRenders[bodyName] = bodyCfg;
    }
  }

  return cfg;
}

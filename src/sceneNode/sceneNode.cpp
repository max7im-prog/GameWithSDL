#include "sceneNode.hpp"
#include "SDL3/SDL.h"
#include "SDL3/SDL_rect.h"
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

          // std::cout <<shapeJson.dump(4);

          auto shapeCfg = std::make_shared<ShapeConfig>();

          {
            auto texture =
                JsonUtils::getOptional<std::string>(shapeJson, "spriteSheet");
            if (texture) {
              shapeCfg->_texture = *texture;
            }
          }

          {
            auto numTextures =
                JsonUtils::getOptional<int>(shapeJson, "numSprites");
            if (numTextures) {
              shapeCfg->_numTextures = *numTextures;
            }
          }

          if (shapeJson.contains("spriteDim")) {
            float w = JsonUtils::getOrDefault(shapeJson["spriteDim"], "w", 0);
            float h = JsonUtils::getOrDefault(shapeJson["spriteDim"], "h", 0);
            shapeCfg->_initialRect = SDL_FRect();
            shapeCfg->_initialRect.h = h;
            shapeCfg->_initialRect.w = w;

            shapeCfg->_initialRect.x = 0;
            shapeCfg->_initialRect.y = 0;
          }

          if (shapeJson.contains("perSpriteOffset")) {
            float x =
                JsonUtils::getOrDefault(shapeJson["perSpriteOffset"], "x", 0);
            float y =
                JsonUtils::getOrDefault(shapeJson["perSpriteOffset"], "y", 0);
            shapeCfg->_offsetPerTexture.x = x;
            shapeCfg->_offsetPerTexture.y = y;
          }

          if (shapeJson.contains("worldDim")) {
            float w = JsonUtils::getOrDefault(shapeJson["worldDim"], "w", 0);
            float h = JsonUtils::getOrDefault(shapeJson["worldDim"], "h", 0);
            shapeCfg->worldSize._h = h;
            shapeCfg->worldSize._w = w;
          }

          bodyCfg->_shapeRenders[shapeName] = shapeCfg;
        }
      }

      cfg->_bodyRenders[bodyName] = bodyCfg;
    }
  }

  return cfg;
}

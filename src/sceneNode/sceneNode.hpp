#pragma once

#include "box2d/math_functions.h"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"
class SceneNode : public RegistryComposite, public virtual IVisitable {
public:
  ~SceneNode() = 0;
  virtual b2Vec2 getWorldPos() = 0;
  const std::unordered_map<std::string, std::weak_ptr<Body>> &getBodies() const;
  const std::unordered_map<std::string, std::weak_ptr<Connection>> &getConnections() const;

protected:
  SceneNode(entt::registry &registry);
  std::unordered_map<std::string, std::weak_ptr<Body>> _bodies;
  std::unordered_map<std::string, std::weak_ptr<Connection>> _connections;
};



class SceneNodeRenderConfig {
public:
  class BodyRenderConfig {
  public:
    class ShapeRenderConfig {
    public:
      using FileName = std::string;
      // TODO: proper implementation
      std::vector<FileName> _textures;
    };

    using ShapeName = std::string;
    std::vector<ShapeName> _renderSequence;
    std::unordered_map<ShapeName, std::shared_ptr<ShapeRenderConfig>> _shapeRenders;
  };

  using BodyName = std::string;
  std::vector<BodyName> _renderSequence;
  std::unordered_map<BodyName, std::shared_ptr<BodyRenderConfig>> _bodyRenders;
};

/**
 * @brief Base class for configurations representing top-level entities.
 *
 * Top-level configurations correspond to independent entities (e.g. creatures,
 * terrain, or objects placed directly in a room) that can be instantiated
 * from JSON definitions.
 */
class SceneNodeConfig : public ObjectConfig {
public:
  SceneNodeConfig() = default;
  virtual ~SceneNodeConfig() = 0;

  /**
   * @brief Defines an object's transform in the world
   *
   */
  struct Transform {
    b2Vec2 _originPos{0, 0};
    b2Vec2 _relativePos{0, 0};
    b2Rot _rootRot{b2MakeRot(0)};
    b2Rot _relativeRot{b2MakeRot(0)};
    float _scaleX{1.0f};
    float _scaleY{1.0f};
    bool _flipX{false};
    bool _flipY{false};
  } _transform;

  /**
   * @brief Optional rendering config, may be nullptr
   *
   */
  std::shared_ptr<SceneNodeRenderConfig> _renderConfig;

  /**
   * @brief Load type-specific configuration values from a JSON object.
   *
   * Implementations should read **only type-specific fields** from the provided
   * JSON structure. Common instance-level transform fields (position, rotation,
   * scale, etc.) are supplied by the room metadata and are **not** expected to
   * appear in the object config JSON.
   *
   * @param json The JSON object containing the type-specific configuration
   * data.
   */
  virtual void fromJSON(const nlohmann::json &json) = 0;

  /**
   * @brief Composes an objects's transform (size scaling, rotation, position,
   * etc.)
   *
   * @param objectJson A json object representing an object's relative transform
   * inside a room. The following field are parsed:
   *  {
   *  "x": 35, // Position is relative to the room's origin
   *  "y": 49, // Position is relative to the room's origin
   *  "scaleX": 60,
   *  "scaleY": 2,
   *  "flipX": false,
   *  "flipY": false,
   *  "rotation": 30, // Degrees
   *  }
   * @param roomJson A json object represinting a room. The following field are
   * parsed:
   *  {
   *  "x": 0, // Room origin
   *  "y": 0, // Room origin
   *  }
   * @return Transform
   */
  static Transform parseObjectTransform(const nlohmann::json &objectJson,
                                        const nlohmann::json &roomJson);
};
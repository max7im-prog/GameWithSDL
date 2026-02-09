#pragma once

#include "SDL3/SDL_rect.h"
#include "box2d/math_functions.h"
#include "commonClasses.hpp"
#include "nlohmann/json_fwd.hpp"
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"

class SceneNodeRenderConfig {
public:
  using ConnectionId = std::string;
  using BodyId = std::string;
  class BodyRenderConfig {
  public:
    class ShapeRenderConfig {
    public:
      using FileName = std::string;
      FileName _texture;
      SDL_FRect _initialRect;
      SDL_FPoint _offsetPerTexture;
      float _initialRotationOffset;
      Uint32 _numTextures;
      struct {
        float _w;
        float _h;
      } _worldSize;
    };

    using ShapeName = std::string;
    std::vector<ShapeName> _renderSequence;
    std::unordered_map<ShapeName, std::shared_ptr<ShapeRenderConfig>>
        _shapeRenders;
  };

  using BodyName = std::string;
  std::vector<BodyName> _renderSequence;
  std::unordered_map<BodyName, std::shared_ptr<BodyRenderConfig>> _bodyRenders;
};
class SceneNode : public RegistryComposite, public virtual IVisitable {
public:
  /**
   * @brief Base class for configurations representing top-level entities.
   *
   * Top-level configurations correspond to independent entities (e.g.
   * creatures, terrain, or objects placed directly in a room) that can be
   * instantiated from JSON definitions.
   */
  class Config : public ObjectConfig {
  public:
    Config() = default;
    virtual ~Config() = 0;

    using Transform = Common::Transform;
    Transform _transform;

    /**
     * @brief Optional rendering config, may be nullptr
     *
     */
    std::shared_ptr<SceneNodeRenderConfig> _renderConfig;

    /**
     * @brief Load type-specific configuration values from a JSON object.
     *
     * Implementations should read **only type-specific fields** from the
     * provided JSON structure. Common instance-level transform fields
     * (position, rotation, scale, etc.) are supplied by the room metadata and
     * are **not** expected to appear in the object config JSON.
     *
     * @param json The JSON object containing the type-specific configuration
     * data.
     */
    virtual void fromJSON(const nlohmann::json &json) = 0;

    /**
     * @brief Composes an objects's transform (size scaling, rotation, position,
     * etc.)
     *
     * @param objectJson A json object representing an object's relative
     * transform inside a room. The following field are parsed:
     *  {
     *  "x": 35, // Position is relative to the room's origin
     *  "y": 49, // Position is relative to the room's origin
     *  "scaleX": 60,
     *  "scaleY": 2,
     *  "flipX": false,
     *  "flipY": false,
     *  "rotation": 30, // Degrees
     *  }
     * @param roomJson A json object represinting a room. The following field
     * are parsed:
     *  {
     *  "x": 0, // Room origin
     *  "y": 0, // Room origin
     *  }
     * @return Transform
     */
    static Transform parseObjectTransform(const nlohmann::json &objectJson,
                                          const nlohmann::json &roomJson);

    static std::shared_ptr<SceneNodeRenderConfig>
    parseRenderConfig(const nlohmann::json &renderConfigJson);
  };

  ~SceneNode() = 0;
  virtual b2Vec2 getWorldPos() = 0;
  const std::unordered_map<std::string, std::weak_ptr<Body>> &getBodies() const;
  const std::unordered_map<std::string, std::weak_ptr<Connection>> &
  getConnections() const;

protected:
  SceneNode(entt::registry &registry);
  std::unordered_map<std::string, std::weak_ptr<Body>> _bodies;
  std::unordered_map<std::string, std::weak_ptr<Connection>> _connections;

  void registerBody(std::weak_ptr<Body> body, std::string_view name);
  void registerConnection(std::weak_ptr<Connection> connection,
                          std::string_view name);
};

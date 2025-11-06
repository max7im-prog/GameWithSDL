#pragma once
#include "objectConfig.hpp"
#include "registryComposite.hpp"
#include "visitor.hpp"
#include "world.hpp"
#include <box2d/box2d.h>

struct TerrainConfig : TopLevelObjectConfig {
  static b2Filter defaultFilter();

  struct BodyParams {
    b2BodyDef _bodyDef;
    b2ShapeDef _shapeDef;
  };

  /**
   * @brief Parses parameters of a body that is a base for a terrain.
   *
   * @param json A json object representing body params. The expected structure
   * is as follows:
   *
   * "bodyParams":
   *  {
   *  "shape":{
   *       "material":{
   *         "friction": 0.5,
   *         "restitution": 0.5,
   *         "rollingResistance": 0
   *     },
   *       "density": 1
   *   },
   *   "body": {
   *     "type": "static",
   *     "linearDamping": 0,
   *     "angularDamping": 0,
   *     "fixedRotation": false
   *   }
   *  }
   * @return BodyParams
   */
  static BodyParams parseBodyParams(const nlohmann::json &json);
};

class Terrain : public RegistryComposite, public virtual IVisitable {
public:
  virtual void update(float dt) override;
  virtual ~Terrain() = 0;

protected:
  Terrain(entt::registry &registry, const std::shared_ptr<World> world);

  const std::shared_ptr<World> world;

private:
  Terrain() = delete;
  Terrain(Terrain &other) = delete;
  Terrain(Terrain &&other) = delete;
  Terrain &operator=(Terrain &other) = delete;
  Terrain &operator=(Terrain &&other) = delete;

  friend class TerrainFactory;
};
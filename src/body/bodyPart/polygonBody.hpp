#include "bodyPart.hpp"
#include "polygon.hpp"

struct PolygonBodyConfig : public BodyPartConfig {
  static PolygonBodyConfig defaultConfig();
  PolygonConfig polygonConfig;
};

class PolygonBody : public BodyPart {
public:

protected:
  PolygonBody() = delete;
  PolygonBody(entt::registry &registry, const std::shared_ptr<World> world,
             const PolygonBodyConfig &config,
             const std::shared_ptr<PhysicsFactory> physicsFactory);
  std::shared_ptr<Polygon> polygon;

  friend class BodyFactory;
};
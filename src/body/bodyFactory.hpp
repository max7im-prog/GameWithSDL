#pragma once
#include "capsuleBody.hpp"
#include "circleBody.hpp"
#include "limbBody.hpp"
#include "physicsFactory.hpp"
#include "polygonBody.hpp"
#include "segmentBody.hpp"
#include "world.hpp"
#include <entt/entt.hpp>
class BodyFactory : public RegistryObjectFactory<BodyFactory> {
public:
  std::shared_ptr<CircleBody> createCircleBody(const CircleBodyConfig &config);
  std::shared_ptr<CapsuleBody>
  createCapsuleBody(const CapsuleBodyConfig &config);
  std::shared_ptr<PolygonBody>
  createPolygonBody(const PolygonBodyConfig &config);
  std::shared_ptr<SegmentBody>
  createSegmentBody(const SegmentBodyConfig &config);

  std::shared_ptr<LimbBody> createLimbBody(const LimbBodyConfig &config);

  BodyFactory(entt::registry &registry, const std::shared_ptr<World> world,
              const std::shared_ptr<PhysicsFactory> physicsFactory);

private:
  void registerBody(std::shared_ptr<Body> body);

  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;
};
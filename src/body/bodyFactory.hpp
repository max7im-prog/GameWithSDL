#pragma once
#include "bodyComponents.hpp"
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

protected:
  template <typename T>
  void attach(std::shared_ptr<T> object, entt::entity ent) {
    auto &comp = registry.emplace_or_replace<PhysicsBody>(ent);
    comp.body = object;
  }

  template <typename T> std::shared_ptr<T> tryCreate(const T::Config &config) {
    return std::shared_ptr<T>(new T(registry, world, config, physicsFactory));
  }

private:
  const std::shared_ptr<World> world;
  const std::shared_ptr<PhysicsFactory> physicsFactory;

  template <typename Derived> friend class RegistryObjectFactory;
};
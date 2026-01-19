#include "body.hpp"
#include "box2d/math_functions.h"
#include "renderComponents.hpp"

Body::~Body() {}

Body::Body(entt::registry &registry, const std::shared_ptr<World> world)
    : Structure(registry), world(world) {}

void Body::performRotation(b2Rot rot) {
  auto curRot = get3dRot();
  if (b2Rot_GetAngle(rot) == b2Rot_GetAngle(curRot)) {
    return;
  }

  _registry.emplace_or_replace<RenderRequiresUpdateTag>(getEntity());
  auto rotIncr = b2InvMulRot(curRot, rot);
  for (auto &[name, shape] : getShapes()) {
    if (auto lock = shape.lock()) {
      b2Rot shapeNewRot = b2MulRot(rotIncr, lock->get3dRot());
      lock->set3dRot(shapeNewRot);
    }
  }
}

#include "renderUpdater.hpp"
#include "box2d/math_functions.h"
#include "renderComponents.hpp"

void RenderUpdater::visit(Shape &shape) {
  auto ent = shape.getEntity();
  if (_registry.all_of<RenderRequiresUpdateTag>(ent)) {
    _registry.remove<RenderRequiresUpdateTag>(ent);
  }

  if (!_registry.all_of<TextureComponent>(ent)) {
    // TODO: log error
    return;
  }

  // Get textures and 3d rotation of a shape
  auto &texture = _registry.get<TextureComponent>(ent);
  b2Rot relativeRot = shape.get3dRot();

  // Change the texture that is chosen for a render based on rotation
  b2Rot initialRot = texture._initialRotationOffset;
  b2Rot compositeRot = b2MulRot(initialRot, relativeRot);

  float angle3d = b2Rot_GetAngle(compositeRot);

  // Adjust to [0, 2pi]
  float adjustedAngle = angle3d + B2_PI;

  int numFaces = texture._numTextures;

  float angleIncr = B2_PI * 2.0f / static_cast<float>(numFaces);

  int currentFace = std::floor(adjustedAngle / angleIncr);

  // Apply changes to the texture component
  texture._currentRect.x =
      texture._offsetPerTexture.x * static_cast<float>(currentFace);
  texture._currentRect.y =
      texture._offsetPerTexture.y * static_cast<float>(currentFace);
}

void RenderUpdater::visit(Body &body) {
  auto ent = body.getEntity();
  if (_registry.all_of<RenderRequiresUpdateTag>(ent)) {
    _registry.remove<RenderRequiresUpdateTag>(ent);
  }

  for (auto &[name, ptr] : body.getShapes()) {
    if (auto lk = ptr.lock()) {
      lk->accept(*this);
    }
  }
}

void RenderUpdater::visit(Creature &creature) {
  auto ent = creature.getEntity();
  if (_registry.all_of<RenderRequiresUpdateTag>(ent)) {
    _registry.remove<RenderRequiresUpdateTag>(ent);
  }

  for (auto &[name, ptr] : creature.getBodies()) {
    if (auto lk = ptr.lock()) {
      lk->accept(*this);
    }
  }
}

void RenderUpdater::visit(DemoCreature &creature) {

  visit(static_cast<Creature &>(creature));
}

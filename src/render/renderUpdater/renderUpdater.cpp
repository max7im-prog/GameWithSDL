#include "renderUpdater.hpp"
#include "box2d/math_functions.h"
#include "renderComponents.hpp"
#include <iostream>

RenderUpdater::RenderUpdater(entt::registry &registry) : _registry(registry) {}

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
  b2Rot initialRot = b2MakeRot(texture._initialRotationOffset);
  b2Rot compositeRot = b2MulRot(initialRot, relativeRot);

  float angle3d = b2Rot_GetAngle(compositeRot);

  // Adjust to [0, 2pi]
  float adjustedAngle = angle3d + B2_PI;

  int numFaces = texture._numTextures;
  float angleIncr = B2_PI * 2.0f / static_cast<float>(numFaces);
  float rotBegin = -angleIncr / 2.0f;
  float angleFromBegin = adjustedAngle - rotBegin;
  int currentFace =
      static_cast<int>(std::floor(angleFromBegin / angleIncr)) % numFaces;

  // Apply changes to the texture component
  texture._currentRect.x =
      texture._offsetPerTexture.x * static_cast<float>(currentFace);
  texture._currentRect.y =
      texture._offsetPerTexture.y * static_cast<float>(currentFace);

  // std::cout << "shape: " << b2Rot_GetAngle(shape.get3dRot()) / B2_PI * 180
  //           << " face: " << currentFace
  //           << " initial rot: " << b2Rot_GetAngle(initialRot) / B2_PI * 180
  //           << " relative rot: " << b2Rot_GetAngle(relativeRot) / B2_PI * 180
  //           << " Composite rot: " << b2Rot_GetAngle(compositeRot) / B2_PI *
  //           180
  //           << std::endl;
}

void RenderUpdater::visit(Body &body) {
  auto ent = body.getEntity();
  if (_registry.all_of<RenderRequiresUpdateTag>(ent)) {
    _registry.remove<RenderRequiresUpdateTag>(ent);
  }

  if (auto lb = dynamic_cast<LimbBody *>(&body)) {
    // std::cout << "limbBody: " << b2Rot_GetAngle(lb->get3dRot()) / B2_PI * 180
    //           << std::endl;
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

  // std::cout << "---------------start--------------------" << std::endl;
  for (auto &[name, ptr] : creature.getBodies()) {
    // std::cout << name << std::endl;
    if (auto lk = ptr.lock()) {
      lk->accept(*this);
    }
  }
  // std::cout << "----------------end---------------------" << std::endl;
}

void RenderUpdater::visit(DemoCreature &creature) {
  auto ent = creature.getEntity();
  auto &seq = _registry.get<RenderSequenceComponent>(creature.getEntity());
  auto rot = creature.getRotation();
  auto angle = b2Rot_GetAngle(rot);

  constexpr std::string_view t = "torso";
  constexpr std::string_view ll = "leftLeg";
  constexpr std::string_view rl = "rightLeg";
  constexpr std::string_view la = "leftArm";
  constexpr std::string_view ra = "rightArm";
  constexpr std::string_view lh = "leftHip";
  constexpr std::string_view rh = "rightHip";
  constexpr std::string_view ls = "leftShoulder";
  constexpr std::string_view rs = "rightShoulder";
  constexpr int NUM_BODIES = 9;

  enum Side { FRONT, LEFT, BACK, RIGHT } curSide;

  if (-B2_PI / 4.0f < angle && angle < B2_PI / 4.0f) {
    curSide = Side::FRONT;
  } else if (-B2_PI * 3.0f / 4.0f < angle && angle < -B2_PI / 4.0f) {
    curSide = Side::RIGHT;
  } else if (B2_PI / 4.0f < angle && angle < B2_PI * 3.0f / 4.0f) {
    curSide = Side::LEFT;
  } else if (-B2_PI * 3.0f / 4.0f < angle || angle > B2_PI * 3.0f / 4.0f) {
    curSide = Side::BACK;
  } else {
    curSide = Side::FRONT;
  }

  seq._renderSequence.clear();
  seq._renderSequence.reserve(NUM_BODIES);

  switch (curSide) {
  case Side::FRONT:
    for (std::string_view sv : {t, ls, rs, lh, rh, ll, rl, la, ra}) {
      seq._renderSequence.emplace_back(sv);
    }

    break;
  case Side::RIGHT:
    for (std::string_view sv : {ra, rs, rh, rl, t, ll, lh, ls, la}) {
      seq._renderSequence.emplace_back(sv);
    }
    break;
  case Side::LEFT:
    for (std::string_view sv : {la, ls, lh, ll, t, rl, rh, rs, ra}) {
      seq._renderSequence.emplace_back(sv);
    }
    break;
  case Side::BACK:
    for (std::string_view sv : {ls, rs, lh, rh, ll, rl, la, ra, t}) {
      seq._renderSequence.emplace_back(sv);
    }
    break;
  }

  visit(static_cast<Creature &>(creature));
}

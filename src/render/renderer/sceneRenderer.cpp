#include "sceneRenderer.hpp"
#include "renderComponents.hpp"
#include "renderUtils.hpp"

SceneRenderer::SceneRenderer(entt::registry &registry,
                             RenderContext &renderContext,
                             std::shared_ptr<TextureManager> mgr)
    : Renderer(renderContext), _registry(registry) {}

void SceneRenderer::visit(SceneNode *n) {
  auto ent = n->getEntity();
  if (!_registry.all_of<RenderSequenceComponent>(ent)) {
  } else {
    return;
    // TODO: log error
  }

  auto renderSeq = _registry.get<RenderSequenceComponent>(ent);

  const auto &bodies = n->getBodies();
  for (auto childId : renderSeq._renderSequence) {
    if (bodies.contains(childId)) {
      auto body = bodies.at(childId).lock();
      if (body) {
        body->accept(*this);
      } else {
        // TODO: log error
      }

    } else {
      // TODO: log error
    }
  }
}

void SceneRenderer::visit(Body *b) {
  auto ent = b->getEntity();
  if (!_registry.all_of<RenderSequenceComponent>(ent)) {
  } else {
    return;
    // TODO: log error
  }

  auto renderSeq = _registry.get<RenderSequenceComponent>(ent);

  const auto &shapes = b->getShapes();
  for (auto childId : renderSeq._renderSequence) {
    if (shapes.contains(childId)) {
      auto shape = shapes.at(childId).lock();
      if (shape) {
        shape->accept(*this);
      } else {
        // TODO: log error
      }

    } else {
      // TODO: log error
    }
  }
}

void SceneRenderer::visit(Shape *s) {
  auto ent = s->getEntity();
  if (!_registry.all_of<TextureComponent, PhysicsShape>(ent)) {
  } else {
    return;
    // TODO: log error
  }

  const auto &shape = _registry.get<PhysicsShape>(ent).shape;
  const auto &tex = _registry.get<TextureComponent>(ent);
  b2Vec2 pos = shape->getWorldPos();

  Common::Transform relativeTransform;
  relativeTransform._originPos = {0, 0};
  relativeTransform._relativePos = pos;
  relativeTransform._rootRot = b2MakeRot(0);
  relativeTransform._relativeRot = shape->getRotation();

  auto &initialTransform = tex._initialTransform;

  auto finalTransform =
      Common::Transform::composeTransform(initialTransform, relativeTransform);

  RenderUtils::renderTexture(tex._texture.get(), tex._currentRect,
                             tex._worldSize._h, tex._worldSize._w,
                             finalTransform, _renderContext);
}

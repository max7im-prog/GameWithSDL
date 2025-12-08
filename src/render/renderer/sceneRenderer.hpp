
#pragma once
#include "entt/entity/fwd.hpp"
#include "renderer.hpp"
#include "sceneNode.hpp"
#include "textureManager.hpp"
#include "visitor.hpp"

class SceneRenderer : public Renderer {
public:
  SceneRenderer(entt::registry &registry, RenderContext &renderContext,
           std::shared_ptr<TextureManager> mgr);


  void visit(SceneNode *n) override;


  void visit(Body *b) override;


  void visit(Shape *s) override;

protected:
  entt::registry& _registry;

private:
};

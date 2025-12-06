#pragma once
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "commonClasses.hpp"
#include "demoCreature.hpp"
#include "entt/entity/fwd.hpp"
#include "fallbackVisitor/fallbackVisitor.hpp"
#include "objectConfig.hpp"
#include "sceneNode.hpp"
#include "textureManager.hpp"
#include "visitor.hpp"

class Texturer : public FallbackVisitor {
public:
  Texturer(entt::registry &registry, RenderContext &renderContext,
           std::shared_ptr<TextureManager> mgr);
  void setupTexturing(std::shared_ptr<SceneNodeRenderConfig> cfg, const Common::Transform& transform);
  void resetTexturing();


  void visit(SceneNode *n) override;


  void visit(Body *b) override;


  void visit(Shape *s) override;

protected:
  entt::registry &_registry;
  RenderContext &_renderContext;
  std::shared_ptr<TextureManager> _textureManager;
  Common::Transform _currentSceneNodeTransform;
  std::shared_ptr<SceneNodeRenderConfig> _currentTopRenderConfig;
  std::shared_ptr<SceneNodeRenderConfig::BodyRenderConfig>
      _currentBodyRenderConfig;
  std::shared_ptr<SceneNodeRenderConfig::BodyRenderConfig::ShapeRenderConfig>
      _currentShapeRenderConfig;

private:
};

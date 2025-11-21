#pragma once
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "demoCreature.hpp"
#include "entt/entity/fwd.hpp"
#include "objectConfig.hpp"
#include "sceneNode.hpp"
#include "textureManager.hpp"
#include "visitor.hpp"

class Texturer : public Visitor {
public:
  Texturer(entt::registry &registry, RenderContext &renderContext,
           std::shared_ptr<TextureManager> mgr);
  void setRenderConfig(std::shared_ptr<TopLevelRenderConfig> cfg);
  void resetRenderConfig();

  void visit(DemoCreature *c) override;
  void visit(CircleTerrain *c) override;
  void visit(PolygonTerrain *c) override;
  void visit(SegmentTerrain *c) override;
  void visit(CapsuleTerrain *c) override;

  void visit(SceneNode *n) override;

  void visit(PolygonBody *b) override;
  void visit(CapsuleBody *b) override;
  void visit(SegmentBody *b) override;
  void visit(CircleBody *b) override;
  void visit(LimbBody *b) override;

  void visit(Body *b) override;

  void visit(Capsule *s) override;
  void visit(Circle *s) override;
  void visit(Polygon *s) override;
  void visit(Segment *s) override;

  void visit(Shape *s) override;

protected:
  entt::registry &_registry;
  RenderContext &_renderContext;
  std::shared_ptr<TextureManager> _textureManager;
  std::shared_ptr<TopLevelRenderConfig> _currentTopRenderConfig;
  std::shared_ptr<TopLevelRenderConfig::BodyRenderConfig>
      _currentBodyRenderConfig;
  std::shared_ptr<TopLevelRenderConfig::BodyRenderConfig::ShapeRenderConfig>
      _currentShapeRenderConfig;

private:
};
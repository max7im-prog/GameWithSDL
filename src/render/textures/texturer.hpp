#pragma once
#include "objectConfig.hpp"
#include "textureManager.hpp"
#include "visitor.hpp"
#include "circle.hpp"
class Texturer : public Visitor {
public:
  Texturer(RenderContext& renderContext,std::shared_ptr<TextureManager> mgr);
  void setRenderConfig(std::shared_ptr<TopLevelRenderConfig> cfg);
  void resetRenderConfig();

  void visit(Circle *c) override;

protected:
  RenderContext& _renderContext;
  std::shared_ptr<TextureManager> _textureManager;
  std::shared_ptr<TopLevelRenderConfig> _currentTopRenderConfig;
  std::shared_ptr<TopLevelRenderConfig::BodyRenderConfig>
      _currentBodyRenderConfig;

private:
};
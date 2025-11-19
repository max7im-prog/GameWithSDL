#include "texturer.hpp"

Texturer::Texturer(RenderContext &renderContext,
                   std::shared_ptr<TextureManager> mgr)
    : _renderContext(renderContext), _textureManager(mgr),
      _currentBodyRenderConfig(nullptr), _currentTopRenderConfig(nullptr) {}

void Texturer::setRenderConfig(std::shared_ptr<TopLevelRenderConfig> cfg) {
  _currentTopRenderConfig = cfg;
}

void Texturer::resetRenderConfig() { _currentTopRenderConfig.reset(); }

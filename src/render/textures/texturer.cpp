#include "texturer.hpp"
#include <iostream>

Texturer::Texturer(RenderContext &renderContext,
                   std::shared_ptr<TextureManager> mgr)
    : _renderContext(renderContext), _textureManager(mgr),
      _currentBodyRenderConfig(nullptr), _currentTopRenderConfig(nullptr) {}

void Texturer::setRenderConfig(std::shared_ptr<TopLevelRenderConfig> cfg) {
  _currentTopRenderConfig = cfg;
}

void Texturer::resetRenderConfig() { _currentTopRenderConfig.reset(); }

void Texturer::visit(DemoCreature *c) {
  std::cout << "textured demo creature" << std::endl;
}
void Texturer::visit(CircleTerrain *c) {
  std::cout << "textured circle terrain" << std::endl;
}
void Texturer::visit(PolygonTerrain *c) {
  std::cout << "textured polygon terrain" << std::endl;
}
void Texturer::visit(SegmentTerrain *c) {
  std::cout << "textured segment terrain" << std::endl;
}
void Texturer::visit(CapsuleTerrain *c) {
  std::cout << "textured capsule terrain" << std::endl;
}
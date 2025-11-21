#include "texturer.hpp"
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "polygonTerrain.hpp"
#include "segmentTerrain.hpp"
#include <iostream>
#include <stdexcept>

Texturer::Texturer(RenderContext &renderContext,
                   std::shared_ptr<TextureManager> mgr)
    : _renderContext(renderContext), _textureManager(mgr),
      _currentBodyRenderConfig(nullptr), _currentTopRenderConfig(nullptr) {}

void Texturer::setRenderConfig(std::shared_ptr<TopLevelRenderConfig> cfg) {
  _currentTopRenderConfig = cfg;
}

void Texturer::resetRenderConfig() { _currentTopRenderConfig.reset(); }

void Texturer::visit(DemoCreature *c) {
  // std::cout << "textured demo creature" << std::endl;
  Texturer::visit(static_cast<Creature *>(c));
}
void Texturer::visit(CircleTerrain *t) {
  // std::cout << "textured circle terrain" << std::endl;
  Texturer::visit(static_cast<Terrain *>(t));
}
void Texturer::visit(PolygonTerrain *t) {
  // std::cout << "textured polygon terrain" << std::endl;
  Texturer::visit(static_cast<Terrain *>(t));
}
void Texturer::visit(SegmentTerrain *t) {
  // std::cout << "textured segment terrain" << std::endl;
  Texturer::visit(static_cast<Terrain *>(t));
}
void Texturer::visit(CapsuleTerrain *t) {
  // std::cout << "textured capsule terrain" << std::endl;
  Texturer::visit(static_cast<Terrain *>(t));
}

void Texturer::visit(Creature *c) {
  if (!_currentTopRenderConfig) {
    throw std::runtime_error("Render config for creature is not set");
  }
  std::cout << "textured creature" << std::endl;
}

void Texturer::visit(Terrain *t) {

  if (!_currentTopRenderConfig) {
    throw std::runtime_error("Render config for terrain is not set");
  }

  auto &renderSequence = _currentTopRenderConfig->_renderSequence;

  auto &bodies = t->getBodies();

  for (auto &bodyName : renderSequence) {
    auto renderIt = _currentTopRenderConfig->_bodyRenders.find(bodyName);
    if (renderIt == _currentTopRenderConfig->_bodyRenders.end()) {
      throw std::runtime_error("No body with name " + bodyName +
                               " found in render sequence");
    }
    auto bodyIt = bodies.find(bodyName);
    if (bodyIt == bodies.end()) {
      throw std::runtime_error("No body with name " + bodyName +
                               " found in bodies");
    }
    _currentBodyRenderConfig = renderIt->second;

    if (auto bodyLock = bodyIt->second.lock()) {
      bodyLock->accept(*this);
    } else {
      // TODO: log error
    }

    _currentBodyRenderConfig.reset();
  }

  std::cout << "textured terrain" << std::endl;
}
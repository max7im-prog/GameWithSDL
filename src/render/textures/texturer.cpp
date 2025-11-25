#include "texturer.hpp"
#include "capsuleTerrain.hpp"
#include "circleTerrain.hpp"
#include "polygonTerrain.hpp"
#include "renderComponents.hpp"
#include "sceneNode.hpp"
#include "segmentTerrain.hpp"
#include <iostream>
#include <stdexcept>

Texturer::Texturer(entt::registry &registry, RenderContext &renderContext,
                   std::shared_ptr<TextureManager> mgr)
    : _registry(registry), _renderContext(renderContext), _textureManager(mgr),
      _currentBodyRenderConfig(nullptr), _currentTopRenderConfig(nullptr) {}

void Texturer::setRenderConfig(std::shared_ptr<SceneNodeRenderConfig> cfg) {
  _currentTopRenderConfig = cfg;
}

void Texturer::resetRenderConfig() { _currentTopRenderConfig.reset(); }


void Texturer::visit(SceneNode *n) {

  if (!_currentTopRenderConfig) {
    throw std::runtime_error("Render config for terrain is not set");
  }

  auto &renderSequence = _currentTopRenderConfig->_renderSequence;

  auto &bodies = n->getBodies();

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

    // Propagate visitor to body
    if (auto bodyLock = bodyIt->second.lock()) {
      _currentBodyRenderConfig = renderIt->second;
      bodyLock->accept(*this);
      _currentBodyRenderConfig.reset();
    } else {
      // TODO: log error
    }
  }
  std::cout << "textured scene node" << std::endl;

  // Attach a render sequence to scene node
  auto ent = n->getEntity();
  _registry.emplace_or_replace<RenderSequenceComponent>(ent, renderSequence);
}


void Texturer::visit(Body *b) {

  if (!_currentBodyRenderConfig) {
    throw std::runtime_error("Render config for body is not set");
  }

  auto &renderSequence = _currentBodyRenderConfig->_renderSequence;

  auto &shapes = b->getShapes();

  for (auto &shapeName : renderSequence) {
    auto renderIt = _currentBodyRenderConfig->_shapeRenders.find(shapeName);
    if (renderIt == _currentBodyRenderConfig->_shapeRenders.end()) {
      throw std::runtime_error("No shape with name " + shapeName +
                               " found in render sequence");
    }

    auto shapeIt = shapes.find(shapeName);
    if (shapeIt == shapes.end()) {
      throw std::runtime_error("No shape with name " + shapeName +
                               " found in shapes");
    }

    // Propagate visitor to shape
    if (auto shapeLock = shapeIt->second.lock()) {
      _currentShapeRenderConfig = renderIt->second;
      shapeLock->accept(*this);
      _currentShapeRenderConfig.reset();
    } else {
      // TODO: log error
    }
  }
  std::cout << "textured body" << std::endl;

  // Attach a render sequence to body
  auto ent = b->getEntity();
  _registry.emplace_or_replace<RenderSequenceComponent>(ent, renderSequence);
}


void Texturer::visit(Shape *s) {
  if (!_currentShapeRenderConfig) {
    throw std::runtime_error("Render config for shape is not set");
  }

  // Attach textures to shape
  auto ent = s->getEntity();
  std::vector<std::shared_ptr<SDL_Texture>> textures;
  for (auto &filename : _currentShapeRenderConfig->_textures) {
    auto t = _textureManager->getTexture(filename);

    if (t) {
      textures.push_back(t);
    } else {
      // TODO: log error
    }
  }
  std::cout << "textured shape" << std::endl;

  _registry.emplace_or_replace<TextureComponent>(ent, textures);
}
#pragma once

#include "commonClasses.hpp"
#include <memory>
class SceneNodeRenderConfig;

struct EntityRequiresTexturingTag {
  std::shared_ptr<SceneNodeRenderConfig> _cfg;
  Common::Transform _sceneNodeOverallTransform;
};

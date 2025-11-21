#pragma once

#include <memory>
class SceneNodeRenderConfig;

struct EntityRequiresTexturingTag {
  std::shared_ptr<SceneNodeRenderConfig> _cfg;
};
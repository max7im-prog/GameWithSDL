#pragma once

#include <memory>
class TopLevelRenderConfig;

struct EntityRequiresTexturingTag {
  std::shared_ptr<TopLevelRenderConfig> _cfg;
};
#include <entt/entt.hpp>
#include "renderContext.hpp"

class RenderBackgroundSystem
{
public:
    RenderBackgroundSystem();
    ~RenderBackgroundSystem();

    void update(entt::registry &registry, const RenderContext &renderContext);
};
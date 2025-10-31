#pragma once
#include "shape.hpp"

struct EmptyShapeConfig : public ShapeConfig {
  void defaultConfig() override;
  void fromJSON(const nlohmann::json &json) override;

private:
  // Not used in EmptyShape
  b2ShapeDef shapeDef;
};

class EmptyShape : public Shape , public Visitable<EmptyShape>{
public:
  using Config = EmptyShapeConfig;
  virtual ~EmptyShape() = default;

protected:
  EmptyShape() = delete;
  EmptyShape(entt::registry &registry, const World &world,
             const EmptyShapeConfig &config);

  friend class ShapeFactory;
};
#pragma once
#include "shape.hpp"

struct EmptyShapeConfig : public ShapeConfig {
  static EmptyShapeConfig defaultConfig();

private:
  // Not used in EmptyShape
  b2ShapeDef shapeDef;
};

class EmptyShape : public Shape {
public:
  using Config = EmptyShapeConfig;
  virtual ~EmptyShape() = default;

protected:
  EmptyShape() = delete;
  EmptyShape(entt::registry &registry, const World &world,
             const EmptyShapeConfig &config);

  friend class ShapeFactory;
};
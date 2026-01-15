#pragma once
#include "shape.hpp"

class EmptyShape : public Shape, public VisitableImpl<EmptyShape> {
public:
  struct Config : public Shape::Config {
    void defaultConfig() override;

  private:
    // Not used in EmptyShape
    b2ShapeDef shapeDef;
  };

  virtual ~EmptyShape() = default;

protected:
  EmptyShape() = delete;
  EmptyShape(entt::registry &registry, const World &world,
             const EmptyShape::Config &config);

  friend class ShapeFactory;
};

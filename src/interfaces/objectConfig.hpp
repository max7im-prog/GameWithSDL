#pragma once
#include <nlohmann/json.hpp>
class ObjectConfig {
public:
  ObjectConfig() = default;
  virtual ~ObjectConfig() = 0;
  virtual void defaultConfig() = 0;
  virtual void fromJSON(const nlohmann::json &json) = 0;
};

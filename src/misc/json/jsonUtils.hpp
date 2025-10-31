#pragma once

#include <nlohmann/json.hpp>

class JsonUtils {
public:
  static std::optional<nlohmann::json> parseJSON(const std::string &filename);

private:
  ~JsonUtils() = delete;
};
#pragma once

#include "box2d/math_functions.h"
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
#include <optional>

class JsonUtils {
public:
  static std::optional<nlohmann::json> parseJSON(const std::string &filename);

  /**
   * @brief Parses the json of a following structure into the b2Vec2:
   * "point": {
   *   "x": 0.5,
   *   "y": 1.0
   * },
   *
   *
   * @param json json to parse
   * @return b2Vec2
   */
  static b2Vec2 parseB2Vec2(const nlohmann::json &json);

  template <typename T>
  static std::optional<T> getOptional(const nlohmann::json &json,
                                      std::string_view key) {
    if (auto it = json.find(key); it != json.end()) {
      try {
        return it->get<T>();
      } catch (const nlohmann::json::exception &) {
        return std::nullopt; // wrong type, etc.
      }
    }
    return std::nullopt; // key missing
  }

  template <typename T>
  static T getOrDefault(const nlohmann::json &json, std::string_view key,
                        const T &defaultValue) {
    if (auto it = json.find(key); it != json.end()) {
      try {
        return it->get<T>();
      } catch (const nlohmann::json::exception &) {
        // TODO: log key not found???
        return defaultValue; // wrong type, fallback
      }
    }
    return defaultValue;
  }

private:
  ~JsonUtils() = delete;
};
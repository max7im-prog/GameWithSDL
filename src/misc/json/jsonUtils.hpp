#pragma once

#include "box2d/math_functions.h"
#include "nlohmann/json_fwd.hpp"
#include <initializer_list>
#include <nlohmann/json.hpp>
#include <optional>

class JsonUtils {
public:
  struct JsonPathStep {
    enum class Kind { Key, Index };
    Kind _kind;
    std::string _key{};
    std::size_t _index{0};

    /**
     * @brief step by key
     */
    static constexpr JsonPathStep k(const std::string &key) {
      return {Kind::Key, key, 0};
    }

    /**
     * @brief step by index
     */
    static constexpr JsonPathStep i(size_t index) {
      return {Kind::Index, {}, index};
    }
  };

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
  static std::optional<T>
  getOptional(const nlohmann::json &json,
              std::initializer_list<JsonPathStep> steps) {

    const nlohmann::json *node = JsonUtils::findNode(json, steps);
    if (!node) {
      return std::nullopt;
    }
    try {
      return node->get<T>();
    } catch (const nlohmann::json::exception &) {
      return std::nullopt;
    }
    return std::nullopt;
  }

  template <typename T>
  static T getOrDefault(const nlohmann::json &json, std::string_view key,
                        const T &defaultValue) {
    if (auto it = json.find(key); it != json.end()) {
      try {
        return it->get<T>();
      } catch (const nlohmann::json::exception &) {
        return defaultValue; // wrong type, fallback
      }
    }
    return defaultValue;
  }

  template <typename T>
  static T getOrDefault(const nlohmann::json &json,
                        std::initializer_list<JsonPathStep> steps,
                        const T &defaultValue) {
    const nlohmann::json *node = JsonUtils::findNode(json, steps);
    if (!node) {
      return defaultValue;
    }
    try {
      return node->get<T>();
    } catch (const nlohmann::json::exception &) {
      return defaultValue;
    }
    return defaultValue;
  }

private:
  static const nlohmann::json *
  findNode(const nlohmann::json &json,
           std::initializer_list<JsonPathStep> steps);
  ~JsonUtils() = delete;
};

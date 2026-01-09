#include "jsonUtils.hpp"
#include "nlohmann/json_fwd.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>

std::optional<nlohmann::json>
JsonUtils::parseJSON(const std::string &filename) {
  try {
    std::ifstream f(filename);
    if (!f.is_open()) {
      return std::nullopt;
    }

    nlohmann::json data = nlohmann::json::parse(f);
    return data;
  } catch (const nlohmann::json::parse_error &e) {
    return std::nullopt;
  } catch (const std::exception &e) {
    return std::nullopt;
  } catch (...) {
    return std::nullopt;
  }
}

b2Vec2 JsonUtils::parseB2Vec2(const nlohmann::json &json) {
  b2Vec2 ret;
  ret.x = JsonUtils::getOrDefault<float>(json, "x", 0.0f);
  ret.y = JsonUtils::getOrDefault<float>(json, "y", 0.0f);
  return ret;
}

const nlohmann::json *
JsonUtils::findNode(const nlohmann::json &json,
                    std::initializer_list<JsonPathStep> steps) {
  const nlohmann::json *current = &json;

  for (const auto &step : steps) {
    switch (step._kind) {
    case JsonPathStep::Kind::Index: {
      if (!current->is_array()) {
        return nullptr;
      }
      if (step._index >= current->size()) {
        return nullptr;
      }
      current = &(current->at(step._index));
      break;
    }
    case JsonPathStep::Kind::Key: {
      if (!current->is_object()) {
        return nullptr;
      }
      auto it = current->find(step._key);
      if (it == current->end()) {
        return nullptr;
      }
      current = &(it.value());
      break;
    }
    }
  }

  return current;
}

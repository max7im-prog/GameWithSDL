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
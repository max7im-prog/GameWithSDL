#include "objectConfig.hpp"
#include "jsonUtils.hpp"


ObjectConfig::~ObjectConfig() = default;



TopLevelObjectConfig::~TopLevelObjectConfig() = default;


TopLevelObjectConfig::Transform
TopLevelObjectConfig::parseObjectTransform(const nlohmann::json &objectJson,
                    const nlohmann::json &roomJson) {
  TopLevelObjectConfig::Transform ret{};

  // Position
  float roomX = JsonUtils::getOrDefault<float>(roomJson, "x", 0.0f);
  float roomY = JsonUtils::getOrDefault<float>(roomJson, "y", 0.0f);
  float offsetX = JsonUtils::getOrDefault<float>(objectJson, "x", 0.0f);
  float offsetY = JsonUtils::getOrDefault<float>(objectJson, "y", 0.0f);
  ret._originPos = {roomX, roomY};
  ret._relativePos = {offsetX, offsetY};

  // Rotation
  float rotationAngle =
      JsonUtils::getOrDefault<float>(objectJson, "rotation", 0);
  float rootAngle = 0;
  ret._relativeRot = b2MakeRot((B2_PI * 2) * rotationAngle / 360.0f);
  ret._rootRot = b2MakeRot((B2_PI * 2) * rootAngle / 360.0f);

  // Scale
  ret._scaleX = JsonUtils::getOrDefault<float>(objectJson, "scaleX", 1.0f);
  ret._scaleY = JsonUtils::getOrDefault<float>(objectJson, "scaleY", 1.0f);

  // Flip
  ret._flipX = JsonUtils::getOrDefault<bool>(objectJson, "flipX", false);
  ret._flipY = JsonUtils::getOrDefault<bool>(objectJson, "flipY", false);

  return ret;
}
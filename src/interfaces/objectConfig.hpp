#pragma once
#include "box2d/math_functions.h"
#include <nlohmann/json.hpp>

/**
 * @brief Abstract base class for all configuration objects.
 *
 * Provides a common interface for configuration types used to initialize
 * or describe various game objects. Every configuration must define its
 * own default values via defaultConfig().
 */
class ObjectConfig {
public:
  ObjectConfig() = default;
  virtual ~ObjectConfig() = 0;

  /**
   * @brief Set default values for this configuration.
   *
   * Implementations should initialize all fields to sensible defaults,
   * ensuring the object is in a valid state.
   */
  virtual void defaultConfig() = 0;
};

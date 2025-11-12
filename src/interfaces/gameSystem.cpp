#include "gameSystem.hpp"

GameSystem::GameSystem(std::string_view name)
    : _name(name), _enabled(true), _updateIntervalSeconds(0.0f),
      _timeSinceLastUpdate(0.0f) {}

GameSystem::~GameSystem() = default;

void GameSystem::setUpdateInterval(double updateIntervalSeconds) {
  _updateIntervalSeconds = updateIntervalSeconds;
  _timeSinceLastUpdate = 0.0;
}

double GameSystem::getUpdateInterval() const { return _updateIntervalSeconds; }

void GameSystem::setEnabled(bool enabled) { _enabled = enabled; }

bool GameSystem::isEnabled() const { return _enabled; }

const std::string &GameSystem::getName() const { return _name; }

bool GameSystem::shouldRun(double secondsPassed) {
  if (!_enabled) {
    return false;
  }
  _timeSinceLastUpdate += secondsPassed;
  if (_updateIntervalSeconds <= 0) {
    _timeSinceLastUpdate = 0.0;
    return true;
  }
  if (_timeSinceLastUpdate >= _updateIntervalSeconds) {
    _timeSinceLastUpdate -= _updateIntervalSeconds;
    return true;
  }
  return false;
}
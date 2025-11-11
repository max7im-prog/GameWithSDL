#include "system.hpp"

System::System(std::string_view name)
    : _name(name), _enabled(true), _updateInterval(0.0f),
      _timeSinceLastUpdate(0.0f) {}

System::~System() = default;

void System::setUpdateInterval(double updateInterval) {
  _updateInterval = updateInterval;
}

double System::getUpdateInterval() const { return _updateInterval; }

void System::setEnabled(bool enabled) { _enabled = enabled; }

bool System::isEnabled() const { return _enabled; }

const std::string &System::getName() const { return _name; }

bool System::shouldRun(Uint64 TPS) {
  if (!_enabled)
    return false;
  _timeSinceLastUpdate += (1.0) / static_cast<double>(TPS);
  if (_updateInterval <= 0) {
    _timeSinceLastUpdate = 0.0;
    return true;
  }
  if (_timeSinceLastUpdate >= _updateInterval) {
    _timeSinceLastUpdate -= _updateInterval;
    return true;
  }
  return false;
}
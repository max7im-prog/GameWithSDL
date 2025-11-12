#pragma once
#include <string>
#include <SDL3/SDL.h>

/**
 * @brief A base class for all the game systems
 *
 */
class GameSystem {
public:
  explicit GameSystem(std::string_view name);
  virtual ~GameSystem() = 0;
  void setUpdateInterval(double updateInterval);
  double getUpdateInterval() const;
  void setEnabled(bool enabled);
  bool isEnabled() const;
  const std::string &getName() const;

protected:
  /**
   * @brief Used to determine whether the system snould run or not. This
   * depends on whether the required update interval has passed or not.
   *
   * Internally advances a counter by the amount of time calculated through TPS.
   *
   * @param secondsPassed - seconds passed from the last call
   */
  bool shouldRun(double secondsPassed);

private:
  const std::string _name;
  bool _enabled;
  double _updateInterval;
  double _timeSinceLastUpdate;
};

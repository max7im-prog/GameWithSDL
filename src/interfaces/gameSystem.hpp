#pragma once
#include <SDL3/SDL.h>
#include <string>

/**
 * @brief A base class for all the game systems
 *
 */
class GameSystem {
public:
  explicit GameSystem(std::string_view name);
  virtual ~GameSystem() = 0;
  virtual void setUpdateInterval(double updateInterval);
  virtual double getUpdateInterval() const;
  void setEnabled(bool enabled);
  bool isEnabled() const;
  const std::string &getName() const;

protected:
  /**
   * @brief Used to determine whether the system snould run or not. This
   * depends on whether the required update interval has passed or not and
   * whether the system is enabled or not.
   *
   * Internally advances a counter by the amount of time passed through a parameter.
   *
   * @param secondsPassed - time passed since the last call in seconds
   */
  bool shouldRun(double secondsPassed);

private:
  const std::string _name;
  bool _enabled;
  double _updateIntervalSeconds;
  double _timeSinceLastUpdate;

  GameSystem(GameSystem &other) = delete;
  GameSystem &operator=(GameSystem &other) = delete;
  GameSystem(GameSystem &&other) = delete;
  GameSystem &operator=(GameSystem &&other) = delete;
};

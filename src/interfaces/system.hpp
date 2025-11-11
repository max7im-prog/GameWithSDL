#pragma once
#include <string>
#include <SDL3/SDL.h>

/**
 * @brief A base class for all the update systema
 *
 */
class System {
public:
  explicit System(std::string_view name);
  virtual ~System() = 0;
  void setUpdateInterval(double updateInterval);
  double getUpdateInterval() const;
  void setEnabled(bool enabled);
  bool isEnabled() const;
  const std::string &getName() const;

protected:
  /**
   * @brief Used to determine whetheer the system snould run or not. This
   * depends on whether the required update interval has passed or not.
   *
   * Internally advances a counter by the amount of time calculated through TPS.
   *
   * @param TPS Ticks Per Second a game runs on.
   */
  bool shouldRun(Uint64 TPS);

private:
  const std::string _name;
  bool _enabled;
  double _updateInterval;
  double _timeSinceLastUpdate;
};

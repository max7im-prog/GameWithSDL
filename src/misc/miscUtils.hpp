#pragma once
#include <memory>
#include <optional>
#include <tuple>
namespace miscUtils {
// Base case: no weak_ptrs
inline std::optional<std::tuple<>> lockAll() {
  return std::make_optional(std::tuple<>());
}

// Recursive variadic template
template <typename T, typename... Rest>
std::optional<std::tuple<std::shared_ptr<T>, std::shared_ptr<Rest>...>>
lockAll(const std::weak_ptr<T> &first, const std::weak_ptr<Rest> &...rest) {
  auto sp = first.lock();
  if (!sp) {
    return std::nullopt; // one expired → whole chain fails
  }
  auto tail = lockAll(rest...);
  if (!tail) {
    return std::nullopt;
  }
  return std::tuple_cat(std::make_tuple(sp), *tail);
}
} // namespace miscUtils
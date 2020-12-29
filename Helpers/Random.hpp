// clang-format off
//
//    Copyright (C) 2020 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#pragma warning(push, 0)
#include <algorithm>  // find()
#include <cstddef>     // size_t
#include <cstdint>    // integer types
#include <random>
#include <type_traits>  // enable_if<>
#include <vector>
#pragma warning(pop)

namespace Random {
  static inline std::intmax_t Get(std::intmax_t min, std::intmax_t max) {
    thread_local std::mt19937 _rnd(std::random_device{}());
    return std::uniform_int_distribution<std::intmax_t>(min, max)(_rnd);
  }
  static inline float Get(float min, float max) {
    thread_local std::mt19937 _rnd(std::random_device{}());
    return std::uniform_real_distribution<float>(min, std::nextafter(max, FLT_MAX))(_rnd);
  }

  static inline auto GetUniqueRange(std::size_t amount, std::intmax_t min, std::intmax_t max) {
    std::vector<std::intmax_t> _ret(amount);
    std::size_t                _idx = 0;

    while (_idx < amount) {
      auto _r = Get(min, max);
      if (std::find(std::cbegin(_ret), std::cend(_ret), _r) == std::cend(_ret)) _ret[_idx++] = _r;
    }

    return _ret;
  }

  template <typename T, std::enable_if_t<std::is_floating_point_v<T>, std::vector<float>> = 0>
  static inline auto GetUniqueRange(std::size_t amount, float min, float max) {
    std::vector<float> _ret(amount);
    std::size_t        _idx = 0;

    while (_idx < amount) {
      auto _r = Get(min, max);
      if (std::find(std::cbegin(_ret), std::cend(_ret), _r) == std::cend(_ret)) _ret[_idx++] = _r;
    }

    return _ret;
  }
}  // namespace Random

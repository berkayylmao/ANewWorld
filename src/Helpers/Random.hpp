// clang-format off
// 
//    ANewWorld (ANewWorld)
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
#include <cstddef>    // size_t
#include <random>
#include <vector>
#pragma warning(pop)

namespace Random {
  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, T> = 0>
  static auto Get(T min, T max) {
    thread_local std::mt19937 _rnd(std::random_device{}());
    return std::uniform_int_distribution<T>(min, max)(_rnd);
  }

  template <typename T, std::enable_if_t<std::is_floating_point_v<T>, T> = 0>
  static auto Get(T min, T max) {
    thread_local std::mt19937 _rnd(std::random_device{}());
    return std::uniform_real_distribution<T>(min, std::nextafter(max, FLT_MAX))(_rnd);
  }

  template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, T> = 0>
  static auto GetUniqueRange(std::size_t amount, T min, T max) {
    std::vector<T> _ret(amount);
    std::size_t    _idx = 0;

    while (_idx < amount) {
      auto _r = Get(min, max);
      if (std::find(std::cbegin(_ret), std::cend(_ret), _r) == std::cend(_ret)) _ret[_idx++] = _r;
    }

    return _ret;
  }
} // namespace Random

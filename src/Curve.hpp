/*******************************************************************************
 * This file is part of FiNe
 * Copyright (C) 2018 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *                    Nina Sartorio (sartorio.nina@gmail.com)
 *
 * FiNe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FiNe is distributed in the hope that it will be useful,
 * but WITOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with FiNe. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file Curve.hpp
 *
 * @brief Representation of a curve as a set of discrete points.
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 */
#ifndef CURVE_HPP
#define CURVE_HPP

#include <cinttypes>

/**
 * @brief Representation of a curve as a set of discrete points.
 */
class Curve {
private:
  /*! @brief Number of points in the curve. */
  const uint_fast32_t _number_of_points;

  /*! @brief x-values. */
  double *_x;

  /*! @brief y-values. */
  double *_y;

public:
  /**
   * @brief Constructor.
   *
   * @param number_of_points Number of points in the curve.
   */
  inline Curve(const uint_fast32_t number_of_points)
      : _number_of_points(number_of_points) {

    _x = new double[_number_of_points];
    _y = new double[_number_of_points];
  }

  /**
   * @brief Destructor.
   */
  inline ~Curve() {
    delete[] _x;
    delete[] _y;
  }

  /**
   * @brief Set the values for the point with the given index.
   *
   * @param index Index.
   * @param x x-value.
   * @param y y-value.
   */
  inline void set_point(const uint_fast32_t index, const double x,
                        const double y) {
    _x[index] = x;
    _y[index] = y;
  }

  /**
   * @brief Get the x-value at the given index.
   *
   * @param index Index.
   * @return Corresponding x-value.
   */
  inline double get_x(const uint_fast32_t index) const { return _x[index]; }

  /**
   * @brief Get the y-value at the given index.
   *
   * @param index Index.
   * @return Corresponding y-value.
   */
  inline double get_y(const uint_fast32_t index) const { return _y[index]; }
};

#endif // CURVE_HPP

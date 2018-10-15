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
 * @file CurveGenerator.hpp
 *
 * @brief Class that generates Curve training data based on a given Function
 * functor.
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 */
#ifndef CURVEGENERATOR_HPP
#define CURVEGENERATOR_HPP

// Curve: an unnormalised discrete representation of a function in a number of
// sampling points, as used by the neural network
#include "Curve.hpp"
// Function: a general representation of a relation that is either described
// mathematically (MathematicalFunction) or by (unstructured) data values
// (DataFunction). The aim of FiNe is to express the second type in terms of
// the first type.
#include "Function.hpp"

// note: we use 'fast' integer types (uint_fast32_t etc.) instead of native
// integer types as this makes more efficient use of the integer CPU registers
// the compiler will convert the given 'fast' type into the most optimal
// native type for the specific architecture we are running on
// on many modern architectures the 'fast' type is always a 64-bit integer,
// irrespective of the type we actually want to use. This is fine as long as
// the 'fast' integer is only used as a temporary variable or if its memory
// footprint is unimportant
// This is a C++11 feature
#include <cinttypes>

/**
 * @brief Class that generates curve training data based on a given Curve
 * functor.
 */
class CurveGenerator {
private:
  /*! @brief Number of points to sample. */
  // this is a 'fast' integer (see note above)
  // we declare it const as we don't want to change it after initialisation
  // this should allow the compiler to optimize loops
  const uint_fast32_t _number_of_points;

  /*! @brief Minimum sample value (inclusive). */
  const double _min_x;

  /*! @brief Sample interval (inclusive). */
  const double _delta_x;

public:
  /**
   * @brief Constructor.
   *
   * @param number_of_points Number of points to sample.
   * @param min_x Minimum sample value (inclusive).
   * @param max_x Maximum sample value (inclusive).
   */
  inline CurveGenerator(const uint_fast32_t number_of_points,
                        const double min_x, const double max_x)
      : _number_of_points(number_of_points), _min_x(min_x),
        _delta_x((max_x - min_x) / (number_of_points - 1)) {}

  /**
   * @brief Generate a Curve from the given Function.
   *
   * @param function Function to use.
   * @return Pointer to a newly created Curve instance.
   */
  inline Curve *generate_curve(const Function &function) const {

    Curve *curve = new Curve(_number_of_points);
    for (uint_fast32_t i = 0; i < _number_of_points; ++i) {
      const double x = _min_x + i * _delta_x;
      curve->set_point(i, x, function(x));
    }
    return curve;
  }
};

#endif // CURVEGENERATOR_HPP

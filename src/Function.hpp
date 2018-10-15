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
 * @file Function.hpp
 *
 * @brief General interface for function representations.
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 */
#ifndef FUNCTION_HPP
#define FUNCTION_HPP

/**
 * @brief General interface for function representations.
 */
class Function {
public:
  /**
   * @brief Get the function value for the given x-value.
   *
   * This is a virtual, non-implemented function, meaning that every Function
   * implementation has to implement its own version of this function.
   *
   * @param x x-value.
   * @return Corresponding y-value.
   */
  virtual double operator()(const double x) const = 0;
};

#endif // FUNCTION_HPP

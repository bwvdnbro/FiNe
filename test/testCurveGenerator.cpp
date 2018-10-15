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
 * @file testCurveGenerator.cpp
 *
 * @brief Unit test for the CurveGenerator class.
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 */

#include "Curve.hpp"
#include "CurveGenerator.hpp"
#include "Function.hpp"

#include <cmath>
#include <fstream>

class TestFunction : public Function {
public:
  /**
   * @brief Test function.
   *
   * @param x x-value.
   * @return Square root of x.
   */
  virtual double operator()(const double x) const { return std::sqrt(x); }
};

/**
 * @brief Unit test for the CurveGenerator class.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return Exit code: 0 on success.
 */
int main(int argc, char **argv) {

  CurveGenerator generator(1000, 0., 1.);

  TestFunction func;
  Curve *curve = generator.generate_curve(func);

  std::ofstream ofile("test_curve_generator.txt");
  ofile << "# x\ty\n";
  for (uint_fast32_t i = 0; i < 1000; ++i) {
    ofile << curve->get_x(i) << "\t" << curve->get_y(i) << "\n";
  }

  // remember to free the Curve memory!
  delete curve;

  return 0;
}

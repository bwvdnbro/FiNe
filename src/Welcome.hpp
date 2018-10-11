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
 * @file Welcome.hpp
 *
 * @brief Welcome message shown at the start of the program.
 *
 * @author Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 */

#include "Configuration.hpp"

#include <iostream>

/**
 * @brief Welcome message shown at the start of the program.
 */
class Welcome {
public:
  /**
   * @brief Print the welcome message to the terminal window.
   */
  inline static void print_welcome() {
    std::cout << "This is NiCe, version " << FINE_VERSION_MAJOR << "."
              << FINE_VERSION_MINOR << std::endl;
  }
};

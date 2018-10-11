################################################################################
# This file is part of FiNe
# Copyright (C) 2018 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
#                    Nina Sartorio (sartorio.nina@gmail.com)
#
# FiNe is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FiNe is distributed in the hope that it will be useful,
# but WITOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with FiNe. If not, see <http://www.gnu.org/licenses/>.
################################################################################

# Rules for files that need to be configured.
message(STATUS "Configuring external data files...")

# Configuration files
configure_file(${PROJECT_SOURCE_DIR}/src/Configuration.hpp.in
               ${PROJECT_BINARY_DIR}/src/Configuration.hpp @ONLY)

add_executable(FiNe FiNe.cpp)

# link to MPI, if we have found it
if(HAVE_MPI)
    target_link_libraries(FiNe ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
endif(HAVE_MPI)

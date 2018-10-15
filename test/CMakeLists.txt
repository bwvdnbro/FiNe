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

### Convenient macros to automate unit test generation #########################
### Do not touch the code below unless you know what you're doing! #############

# Add a new unit test
# A new target with the test sources is constructed, and a CTest test with the
# same name is created. The new test is also added to the global list of test
# contained in the check target
macro(add_unit_test)
    set(options PARALLEL)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(TEST "${options}" "${oneValueArgs}"
                               "${multiValueArgs}" ${ARGN})
    message(STATUS "generating ${TEST_NAME}")
    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${TEST_SOURCES})
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/rundir/test)
    target_link_libraries(${TEST_NAME} ${TEST_LIBS})

    if(TEST_PARALLEL AND HAVE_MPI)
      set(TESTCOMMAND ${MPIEXEC})
      set(TESTARGS ${MPIEXEC_NUMPROC_FLAG} 3 ${MPIEXEC_PREFLAGS}
                   "./${TEST_NAME}" ${MPIEXEC_POSTFLAGS})
      set(TESTCOMMAND ${TESTCOMMAND} ${TESTARGS})
    else(TEST_PARALLEL AND HAVE_MPI)
      set(TESTCOMMAND ${TEST_NAME})
    endif(TEST_PARALLEL AND HAVE_MPI)
    add_test(NAME ${TEST_NAME}
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/rundir/test
             COMMAND ${TESTCOMMAND})

    set(TESTNAMES ${TESTNAMES} ${TEST_NAME})

    # create a custom target for this test alone
    add_custom_target(check_${TEST_NAME}
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                              -R ${TEST_NAME}
                      DEPENDS ${TEST_NAME})
endmacro(add_unit_test)

set(SOURCE_FOLDER "${PROJECT_SOURCE_DIR}/src")

### Actual unit test generation ################################################
### Add new unit tests below ###################################################

## Unit test for welcome message (use this as template for new unit tests)
set(TESTWELCOME_SOURCES
    testWelcome.cpp

    ${SOURCE_FOLDER}/Welcome.hpp)
add_unit_test(NAME testWelcome
              SOURCES ${TESTWELCOME_SOURCES})

## Unit test for CurveGenerator
set(TESTCURVEGENERATOR_SOURCES
    testCurveGenerator.cpp

    ${SOURCE_FOLDER}/Curve.hpp
    ${SOURCE_FOLDER}/CurveGenerator.hpp
    ${SOURCE_FOLDER}/Function.hpp)
add_unit_test(NAME testCurveGenerator
              SOURCES ${TESTCURVEGENERATOR_SOURCES})

### Done adding unit tests. Create the 'make check' target #####################
### Do not touch these lines unless you know what you're doing! ################
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                        DEPENDS ${TESTNAMES})

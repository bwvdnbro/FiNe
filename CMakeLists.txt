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

## Project initialization ######################################################

# the most advanced feature we use is the ProcessorCount module, which is only
# available since CMake 2.8.5
cmake_minimum_required(VERSION 2.8.5)

# Set the default CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE INTERNAL
        "Set the default CMake build type to Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(FiNe)

# Enable unit testing using CTest
enable_testing()

## Macro definitions ###########################################################

# Add minimal cpp file for flag testing
execute_process(COMMAND ${CMAKE_COMMAND} -E echo
                "int main(int argc, char **argv){ return 0; }"
                OUTPUT_FILE ${PROJECT_BINARY_DIR}/mintest.cpp)

# Macro used to safely add compiler flags (to the C/CXX compiler):
# we try to add the compiler flag and then compile a minimal cpp file
# if this succeeds, we keep the flag
# if it fails, we reset the flags, and throw an error/warning depending on the
# value of the FATAL parameter
macro(add_compiler_flag FLAG FATAL)
  set(BACKUP_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
  try_compile(FLAG_WORKS ${PROJECT_BINARY_DIR}
                         ${PROJECT_BINARY_DIR}/mintest.cpp)
  if(NOT FLAG_WORKS)
    if(${FATAL} STREQUAL "REQUIRED")
      message(FATAL_ERROR "The compiler does not support the ${FLAG} flag!")
    else(${FATAL} STREQUAL "REQUIRED")
      message(STATUS "Not using unsupported compiler flag ${FLAG}.")
      set(CMAKE_CXX_FLAGS ${BACKUP_CXX_FLAGS})
    endif(${FATAL} STREQUAL "REQUIRED")
  endif(NOT FLAG_WORKS)
endmacro(add_compiler_flag)

## System configuration checks #################################################

# The version number.
set(FINE_VERSION_MAJOR 0)
set(FINE_VERSION_MINOR 1)

# Enable C++11, since we need this for delegating constructors
add_compiler_flag("-std=c++11" REQUIRED)

# Enable all standard compiler warnings and enforce them
add_compiler_flag("-Wall -Werror" OPTIONAL)

# Enable the address sanitizer in debug builds
# (to symbolize the code, run
#   export ASAN_SYMBOLIZER_PATH=<path to llvm-symbolizer>
#   export ASAN_OPTIONS=symbolize=1
#  before running the code)
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_compiler_flag("-fsanitize=address -fno-omit-frame-pointer" OPTIONAL)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# If not explicitly set, try to find the number of cores on the system
include(ProcessorCount)
ProcessorCount(MAX_NUMBER_OF_THREADS)
if(MAX_NUMBER_OF_THREADS EQUAL 0)
  # If we cannot determine the number of cores, we set it to a safe large
  # value
  message(WARNING
          "Cannot determine core count on this system, using value 128")
  set(MAX_NUMBER_OF_THREADS 128)
endif(MAX_NUMBER_OF_THREADS EQUAL 0)
set(MAX_NUM_THREADS ${MAX_NUMBER_OF_THREADS})

# If we have multiple cores available: find OpenMP.
if(MAX_NUM_THREADS GREATER 1)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    set(HAVE_OPENMP True)
    message(STATUS "OpenMP found. Shared memory parallelization will work.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  else(OPENMP_FOUND)
    set(HAVE_OPENMP False)
    message(WARNING
            "OpenMP not found. Shared memory parallelization will not work.")
  endif(OPENMP_FOUND)
else(MAX_NUM_THREADS GREATER 1)
  set(HAVE_OPENMP False)
  message(WARNING "Only 1 core available, so not enabling OpenMP support.")
endif(MAX_NUM_THREADS GREATER 1)

# Find MPI
find_package(MPI)
if(MPI_CXX_FOUND)
  set(HAVE_MPI True)
  message(STATUS "MPI found. Distributed memory parallelization will work.")
  message(STATUS "MPI exec: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} PROCS"
                 "${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
  include_directories(${MPI_C_INCLUDE_PATH})
  include_directories(${MPI_CXX_INCLUDE_PATH})
else(MPI_CXX_FOUND)
  set(HAVE_MPI False)
  message(WARNING
          "MPI not found. Distributed memory parallelization will not work.")
endif(MPI_CXX_FOUND)

# Check if we need to enable architecture specific CPU instructions
if(ACTIVATE_ARCH_NATIVE)
  add_compiler_flag("-march=native" OPTIONAL)
endif(ACTIVATE_ARCH_NATIVE)

## Code configuration ##########################################################

# Tell CMake that headers are in one of the src folders.
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR}/src)

# Set the path where the generated executables are stored.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/rundir)

# Set the path where the generated libraries are stored
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Enter the folder containing the source files and configure the executables.
add_subdirectory(src)

# Enter the folder containing the unit tests
add_subdirectory(test)

## Documentation configuration #################################################

# Generate documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
  message(STATUS
          "Doxygen found! You can generate documentation using 'make doc'")
  if(DOXYGEN_DOT_FOUND)
    set(HAVE_DOT YES)
  else(DOXYGEN_DOT_FOUND)
    set(HAVE_DOT NO)
  endif(DOXYGEN_DOT_FOUND)
  # Configure the Doxyfile (sets the correct output path and Dot path)
  configure_file(${PROJECT_SOURCE_DIR}/Doxyfile.in
                 ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)
  # Add a command that will generate the doxygen documentation
  add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/doc/html/index.html
                     COMMAND ${DOXYGEN_EXECUTABLE}
                             ${PROJECT_BINARY_DIR}/Doxyfile
                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     COMMENT "Generating API documentation with Doxygen"
                     VERBATIM)
  # Add a custom target that calls the command defined above
  add_custom_target(doc DEPENDS ${PROJECT_BINARY_DIR}/doc/html/index.html)
else(DOXYGEN_FOUND)
  # Could not find doxygen. Inform the user that documentation will not be
  # available.
  message(WARNING "Doxygen not found. 'make doc' will not work!")
endif(DOXYGEN_FOUND)

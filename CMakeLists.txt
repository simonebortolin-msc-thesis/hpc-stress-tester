cmake_minimum_required(VERSION 3.11)
project(hpc_stress_tester)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -lm -O3 -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(mpi_stress_tester main.cpp countdown.cpp eratostene.cpp pi.cpp)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(mpi_stress_tester PUBLIC OpenMP::OpenMP_CXX)
endif()

# MPI
find_package(MPI REQUIRED)
target_link_libraries(mpi_stress_tester PUBLIC MPI::MPI_CXX)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

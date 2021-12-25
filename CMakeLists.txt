cmake_minimum_required(VERSION 2.8.12.2)
project(heuristic_ckc)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "-std=c++11")

#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

add_executable(heuristic_ckc src/main.cpp src/util/Utils.cpp src/util/Utils.h src/CkcSolver.cpp src/CkcSolver.h src/model/Center.cpp src/model/Center.h src/model/KCSolution.cpp src/model/KCSolution.h nlohmann/json.hpp)

# Require MPI for this project:
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})

target_link_libraries(heuristic_ckc ${MPI_LIBRARIES})
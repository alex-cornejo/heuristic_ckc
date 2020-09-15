cmake_minimum_required(VERSION 3.16)
project(heuristic_ckc)

set(CMAKE_CXX_STANDARD 14)

#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
include(build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(heuristic_ckc src/main.cpp src/util/Utils.cpp src/util/Utils.h src/CkcSolver.cpp src/CkcSolver.h src/model/Center.cpp src/model/Center.h src/model/KCSolution.cpp src/model/KCSolution.h)

target_link_libraries(heuristic_ckc ${CONAN_LIBS})
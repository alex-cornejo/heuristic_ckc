cmake_minimum_required(VERSION 3.16)
project(ckc_heuristic)

set(CMAKE_CXX_STANDARD 14)

#SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
include(build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(ckc_heuristic src/main.cpp src/util/Utils.cpp src/util/Utils.h src/CkcSolver.cpp src/CkcSolver.h src/model/Center.cpp src/model/Center.h src/model/KCSolution.cpp src/model/KCSolution.h)

target_link_libraries(ckc_heuristic ${CONAN_LIBS})
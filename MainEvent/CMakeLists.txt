set (CMAKE_CXX_STANDARD 11)

cmake_minimum_required (VERSION 2.8)

project (nurc_lax)

find_package(OpenCV REQUIRED)

find_package(CUDA REQUIRED)

find_library(DARKNET_LIB darknet /home/nvidia/nurc-lax-robot/MainEvent/ML)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("${CUDA_INCLUDE_DIRS}") 

add_executable(cv_nurc_lax main.cpp)

target_link_libraries(cv_nurc_lax ${OpenCV_LIBS})

target_link_libraries(cv_nurc_lax ${DARKNET_LIB})

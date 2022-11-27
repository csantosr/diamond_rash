cmake_minimum_required(VERSION 3.23)
project(PathFinder)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(
        a-star
        AStar/AStar.hpp
        AStar/AStar.cpp
)

add_executable(PathFinder main.cpp)

target_link_libraries(PathFinder PRIVATE a-star ${OpenCV_LIBS})
#target_link_libraries(PathFinder  ${OpenCV_LIBS})
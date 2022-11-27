cmake_minimum_required(VERSION 3.23)
project(BoardParser)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(BoardParser main.cpp)
target_link_libraries( BoardParser  ${OpenCV_LIBS})

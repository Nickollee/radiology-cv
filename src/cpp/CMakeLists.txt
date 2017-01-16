cmake_minimum_required(VERSION 2.8.12)

project(radiology-cv)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Radiograph Radiograph.cpp)
target_link_libraries(Radiograph ${OpenCV_LIBS})
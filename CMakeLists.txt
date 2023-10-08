cmake_minimum_required(VERSION 3.10)

project(opencv_example_project)

set(OpenCV_DIR "/home/csy2022/opencv/opencv-4.5.4/build")

find_package(OpenCV REQUIRED)

add_executable(demo test.cpp)

target_link_libraries(demo PRIVATE ${OpenCV_LIBS})


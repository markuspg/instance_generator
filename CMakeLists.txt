cmake_minimum_required(VERSION 3.13.2 FATAL_ERROR)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED 14)

add_executable(instance_generator
    src/exceptions.cpp
    src/histogram.cpp
    src/main.cpp
    src/mainwindow.cpp)
target_link_libraries(instance_generator
    Qt5::Widgets)

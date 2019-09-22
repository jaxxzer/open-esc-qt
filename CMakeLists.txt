cmake_minimum_required(VERSION 3.5)

project(open-esc-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS SerialPort REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/lib/ping-cpp/src/message)

add_executable(open-esc-qt
    com-handle.cpp
    com-handle.h
    com-parser.cpp
    com-parser.h
    device.cpp
    device.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    port-scanner.cpp
    port-scanner.h
    qcustomplot.h
    qcustomplot.cpp
)

target_link_libraries(open-esc-qt PRIVATE Qt5::Widgets Qt5::SerialPort Qt5::PrintSupport)

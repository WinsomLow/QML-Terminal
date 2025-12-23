# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2)

set(CMAKE_AUTOMOC ON)

qt_add_executable(qml_terminal 
    main.cpp
    terminal_backend.cpp
    terminal_backend.h
)

qt_add_qml_module(qml_terminal
    URI QmlTerminal
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_link_libraries(qml_terminal PRIVATE Qt6::Qml Qt6::Quick Qt6::QuickControls2)

if(UNIX AND NOT APPLE)
    target_link_libraries(qml_terminal PRIVATE util)
endif()

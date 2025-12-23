cmake_minimum_required(VERSION 3.22)

find_package(Qt6 REQUIRED COMPONENTS Quick)

set(CMAKE_AUTOMOC ON)

qt_add_executable(helloqml WIN32
    main.cpp
)

qt_add_qml_module(helloqml
    URI HelloQml
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_link_libraries(helloqml PRIVATE Qt6::Quick)

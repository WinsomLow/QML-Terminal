cmake_minimum_required(VERSION 3.22)

# === Configuration ===
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_WARNING_FLAGS)
if(MSVC)
    list(APPEND COMMON_WARNING_FLAGS /W4)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND COMMON_WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND COMMON_WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wold-style-cast
    )
endif()

if(COMMON_WARNING_FLAGS)
    add_compile_options(${COMMON_WARNING_FLAGS})
endif()

# === Project ===
project(GeneralCppTemplate VERSION 0.1.0 LANGUAGES CXX)

# Keep built binaries and libraries organized.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# === Subdirectories ===
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(pocs)
add_subdirectory(include)

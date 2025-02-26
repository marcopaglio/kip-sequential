cmake_minimum_required(VERSION 3.30)
project(kip_sequential)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(kip_sequential_lib
        src/Pixel.cpp
        src/Pixel.h
        src/Image.cpp
        src/Image.h
        src/Kernel.cpp
        src/Kernel.h
        src/ImageReader.cpp
        src/ImageReader.h
        src/STBImageReader.cpp
        src/STBImageReader.h
)

add_compile_definitions(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

enable_testing()

add_subdirectory(tests)


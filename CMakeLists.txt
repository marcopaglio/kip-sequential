cmake_minimum_required(VERSION 3.30)
project(kip_sequential)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND USE_ASAN)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_compile_options(/fsanitize=address /Zi)
endif()

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
        src/ImageProcessing.cpp
        src/ImageProcessing.h
        src/KernelFactory.cpp
        src/KernelFactory.h
        src/Timer.cpp
        src/Timer.h
        src/HighResolutionTimer.cpp
        src/HighResolutionTimer.h
        src/SteadyTimer.cpp
        src/SteadyTimer.h
)

add_executable(kip_sequential_main src/main.cpp)

target_link_libraries(kip_sequential_main kip_sequential_lib)

add_compile_definitions(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

enable_testing()

add_subdirectory(tests)


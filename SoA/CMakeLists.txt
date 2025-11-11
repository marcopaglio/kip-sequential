cmake_minimum_required(VERSION 3.30)
project(kip_sequential)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND USE_ASAN)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_compile_options(/fsanitize=address /Zi)
endif()

include_directories(
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/src
)

add_library(kip_sequential_lib
        src/image/Image.cpp
        src/image/Image.h
        src/kernel/Kernel.cpp
        src/kernel/Kernel.h
        src/image/reader/ImageReader.cpp
        src/image/reader/ImageReader.h
        src/image/reader/STBImageReader.cpp
        src/image/reader/STBImageReader.h
        src/processing/ImageProcessing.cpp
        src/processing/ImageProcessing.h
        src/kernel/KernelFactory.cpp
        src/kernel/KernelFactory.h
)

add_executable(kip_sequential_main
        src/expt/main.cpp
        src/expt/timer/Timer.cpp
        src/expt/timer/Timer.h
        src/expt/timer/HighResolutionTimer.cpp
        src/expt/timer/HighResolutionTimer.h
        src/expt/timer/SteadyTimer.cpp
        src/expt/timer/SteadyTimer.h
)
target_link_libraries(kip_sequential_main kip_sequential_lib)

add_executable(kip_sequential_profile src/expt/profile.cpp)
target_link_libraries(kip_sequential_profile kip_sequential_lib)

add_compile_definitions(IMAGES_INPUT_DIRPATH="${PROJECT_SOURCE_DIR}/../images/input/")
add_compile_definitions(IMAGES_OUTPUT_DIRPATH="${PROJECT_SOURCE_DIR}/../images/output/")
add_compile_definitions(CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}")

enable_testing()

add_subdirectory(tests)
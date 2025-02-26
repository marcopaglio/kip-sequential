include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
        runAllTests.cpp
        PixelTest.cpp
        KernelTest.cpp
)

add_executable(kip_sequential_runTests ${TEST_SOURCES})

target_link_libraries(kip_sequential_runTests kip_sequential_lib gtest_main)

add_test(NAME runTests COMMAND runTests)
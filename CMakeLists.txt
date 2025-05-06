cmake_minimum_required(VERSION 3.28)
project(TapeSorter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MINGW)
    add_link_options(-lstdc++fs)
endif()

add_executable(TapeSorter
        main.cpp
        include/TapeSorter.h
        include/TapeDevice.h
        include/config.h
        src/TapeSorter.cpp
        src/FileTapeDevice.cpp
        src/config.cpp
)

add_subdirectory(external/googletest-1.17.0)
include_directories(external/googletest-1.17.0/googletest/include)

enable_testing()

add_executable(tests
        src/TapeSorter.cpp
        src/FileTapeDevice.cpp
        src/config.cpp
        test/tape_sorter_tests.cpp
        test/file_tape_device_tests.cpp
)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests gtest gtest_main)
add_test(NAME TapeSorterTest COMMAND tests)

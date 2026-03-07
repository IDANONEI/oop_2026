cmake_minimum_required(VERSION 3.20)
project(testCline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)


add_executable(testCline_app
        2.3.cpp
        VectorUtils.cpp
        VectorUtils.h
        ../oop2_3_2/Dictionary.cpp
        ../oop2_3_2/Dictionary.h
        ../oop2_3_2/ConsoleUI.cpp
        ../oop2_3_2/ConsoleUI.h
        ../oop2_3_2/main.cpp
)

target_include_directories(testCline_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testCline_tests
        tests/test_vectorutils.cpp
        VectorUtils.cpp
        VectorUtils.h
)

target_include_directories(testCline_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(testCline_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
enable_testing()
include(Catch)
catch_discover_tests(testCline_tests)
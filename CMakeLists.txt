cmake_minimum_required(VERSION 3.20)
project(oop_2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

include(Catch)

#add_subdirectory(oop2_3_2)
#add_subdirectory(concept)
#add_subdirectory(oop2_5_1)
#add_subdirectory(oop2_6)
add_subdirectory(oop3_1_2)

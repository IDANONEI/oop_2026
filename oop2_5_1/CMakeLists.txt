add_executable(oop2_5_1
        parsUrl.cpp
        ParseUrlModules.cpp
        ParseUrlModules.h
)

target_include_directories(oop2_5_1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop2_5_1_tests
        test/testParsUrl.cpp
        ParseUrlModules.cpp
        ParseUrlModules.h
)

target_include_directories(oop2_5_1_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop2_5_1_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop2_5_1_tests)
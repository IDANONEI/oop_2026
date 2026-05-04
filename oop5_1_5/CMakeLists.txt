add_executable(oop5_1_5
        CDate.h
        CDate.cpp
)

target_include_directories(oop5_1_5 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop5_1_5_tests
        test/DateTest.cpp
        CDate.cpp
        CDate.h


)

target_include_directories(oop5_1_5_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop5_1_5_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop5_1_5_tests)


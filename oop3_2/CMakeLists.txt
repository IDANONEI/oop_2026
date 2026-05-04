add_executable(oop3_2
        main.cpp
        Variable.cpp
        Function.cpp
        Calculator.cpp
        ControllerCalculator.cpp
)

target_include_directories(oop3_2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop3_2_tests
        test/testCalculator.cpp
        test/testOptimization.cpp
        Calculator.cpp
        Variable.cpp
        Function.cpp
)

target_include_directories(oop3_2_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop3_2_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop3_2_tests)


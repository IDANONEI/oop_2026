add_executable(oop3_1_2
        main.cpp
        Car.cpp
        Car.h
        CarController.cpp
        CarController.h
)

target_include_directories(oop3_1_2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop3_1_2_tests
        test/testCar.cpp
        Car.cpp
        CarController.cpp
)

target_include_directories(oop3_1_2_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop3_1_2_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop3_1_2_tests)


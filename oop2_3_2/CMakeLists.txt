add_executable(oop2_3_2
        main.cpp
        ConsoleUI.cpp
        Dictionary.cpp
)

target_include_directories(oop2_3_2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop2_3_2_tests
        test/testDict.cpp
        Dictionary.cpp
        ConsoleUI.cpp
        test/testUI.cpp
)

target_include_directories(oop2_3_2_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop2_3_2_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop2_3_2_tests)
add_executable(oop3_3
        Actor.cpp
        BankedActor.cpp
        Bank.cpp
        Actors.cpp
        Simulation.cpp
)

target_include_directories(oop3_3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop3_3_tests
        test/testBank.cpp
        Actor.cpp
        BankedActor.cpp
        Bank.cpp
        Actors.cpp
        Simulation.cpp
)

target_include_directories(oop3_3_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop3_3_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop3_3_tests)


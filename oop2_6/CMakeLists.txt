add_executable(oop2_6
        expandTemplate.cpp
        trie.cpp
        trie.h
        processor.cpp
        processor.h
)

target_include_directories(oop2_6 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(oop2_6_tests
        test/testExpandT.cpp
        trie.cpp
        trie.h
        processor.cpp
        processor.h
)

target_include_directories(oop2_6_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop2_6_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop2_6_tests)
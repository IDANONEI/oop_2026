cmake_minimum_required(VERSION 3.16)
project(oop4_1)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(oop4_1
        main.cpp
        CCircle.cpp
        CLineSegment.cpp
        CRectangle.cpp
        CTriangle.cpp
        ICanvasDrawable.h
        ICanvas.h
        CCanvas.cpp
        CCanvas.h
        CShapeStorage.cpp
        CShapeStorage.h
)

target_include_directories(oop4_1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop4_1 PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

add_executable(oop4_1_tests
        test/CCircleTests.cpp
        test/CLineSegmentTests.cpp
        test/CPointTests.cpp
        test/CRectangleTests.cpp
        test/CTriangleTests.cpp
        CCircle.cpp
        CLineSegment.cpp
        CRectangle.cpp
        CTriangle.cpp
        ICanvasDrawable.h
        CShapeStorage.cpp
        CShapeStorage.h
        test/StoreTest.cpp
)

target_include_directories(oop4_1_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(oop4_1_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(oop4_1_tests)
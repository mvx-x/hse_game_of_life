cmake_minimum_required(VERSION 3.15)
project(GameOfLife VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS Graphics Window System REQUIRED)
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_library(GameOfLifeLib src/game_of_life.cpp)
target_include_directories(GameOfLifeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(GameOfLifeLib PUBLIC 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE GameOfLifeLib)

add_executable(GameOfLifeTests src/game_of_life_test.cpp)
target_link_libraries(GameOfLifeTests PRIVATE 
    GameOfLifeLib 
    GTest::gtest_main
)
gtest_discover_tests(GameOfLifeTests)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Minesweeper)

set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR C:/SFML/lib/cmake/SFML)

find_package(SFML 2.5.1 COMPONENTS system window graphics audio network REQUIRED)
add_executable(Minesweeper main.cpp
        textbox.h
        Boardstuff.h
        Board.h
        Board.h
        Square.h
)

include_directories(c:/SFML/include/SFML)

target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio)
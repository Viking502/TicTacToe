cmake_minimum_required(VERSION 3.13)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 14)

add_executable(TicTacToe
        main.cpp
        Tile.cpp
        Boot.cpp
        Game.cpp
        )

target_link_libraries(${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system)

cmake_minimum_required(VERSION 3.23)
project(07l_challenge_tile_game)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(07l_challenge_tile_game
        main.cpp TileGame.cpp TileGame.h)

//
// Created by Griffin Thompson on 10/27/22.
//

#include "TileGame.h"

#include <array>
#include <optional>
#include <tuple>

#include <random>
#include <iostream>
#include <sstream>

// change this to whatever you want really.
#define MAXSHUFFLE 1

using std::optional;
using std::nullopt;

TileGame::TileGame() {
    generateTileSurface();
#ifndef NDEBUG
    debugPrintTileSurface();
    debugPrintTileArray();
#endif
}

Result TileGame::moveTile(int predicate, Direction direction) {
    auto coords = where({predicate});

    switch (direction) {
        case Up:
            return moveUp(coords);
        case Down:
            return moveDown(coords);
        case Left:
            return moveLeft(coords);
        case Right:
            return moveRight(coords);
    }
}

bool TileGame::isSorted() {
    size_t k = 0;
    array<int, ROWS * COLUMNS - 1> arr{};

    // flattening the 2d array
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMNS; j++) {
            if (!tiles[i][j].has_value())
                continue;
            arr[k] = tiles[i][j].value();
            k++;
        }
    }

    return std::is_sorted(arr.begin(), arr.end());
}

void TileGame::generateTileSurface() {
    // the double braces at the beginning are required because std::array literals are weird.
    TileGrid arr =
            {{
                     {{{}, {1}, {2}}},
                     {{{3}, {4}, {5}}},
                     {{{6}, {7}, {8}}}
             }};

    tiles = arr;

    // generates a random number between 1 and 800 inclusive
    const size_t maxShuffleMoves = MAXSHUFFLE;
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<size_t> moveNumDistrib(1, maxShuffleMoves);

    size_t numOfMoves = moveNumDistrib(gen);

    // 0 = up
    // 1 = down
    // 2 = left
    // 3 = right
    std::uniform_int_distribution<size_t> directionDistribution(0, 3);

    std::uniform_int_distribution<int> numToMove(1, 8);

    // doing things this way ensures that there's always a way back to the sorted tile grid.
    for (size_t _ = 0; _ < numOfMoves; _++) {
        auto num = numToMove(gen);
        std::cout << "TileGame::generateTileSurface - num is: " << num << std::endl;
        auto coord = where({num});

        auto direction = directionDistribution(gen);
        std::cout << "TileGame::generateTileSurface - direction is: " << direction  << std::endl;
        //auto direction = 2;
        if (direction == 0)
            moveUp(coord);
        else if (direction == 1)
            moveDown(coord);
        else if (direction == 2)
            moveLeft(coord);
        else if (direction == 3)
            moveRight(coord);
    }
}

TileGame::CoordinatePair TileGame::where(optional<int> predicate) {
    // searching for some number
    if (predicate.has_value()) {
        for (size_t i = 0; i < tiles.size(); i++) {
            for (size_t j = 0; j < tiles[i].size(); j++) {
                if (tiles[i][j].has_value())
                    if (tiles[i][j].value() == predicate)
                        return {i, j}; // tuple literal
            }
        }
    }
        // searching for nullopt
    else {
        for (size_t i = 0; i < tiles.size(); i++) {
            for (size_t j = 0; j < tiles[i].size(); j++) {
                if (!tiles[i][j].has_value())
                    return {i, j};
            }
        }
    }
    // we should never reach this spot
    return {-1, -1};
}

Result TileGame::moveUp(CoordinatePair coords) {
    if (!canMove(coords, Up)) return Fail;

    size_t x, newX, y;
    std::tie(newX, y) = coords;
    x = std::get<0>(coords);
    // go up a row
    newX -= 1;

    optional<int> temp = tiles[x][y];

    tiles[x][y] = nullopt;
    tiles[newX][y] = temp;

    return Ok;
}

Result TileGame::moveDown(CoordinatePair coords) {
    if (!canMove(coords, Down)) return Fail;

    size_t x, newX, y;
    std::tie(newX, y) = coords;
    x = std::get<0>(coords);
    // go down a row
    newX += 1;

    optional<int> temp = tiles[x][y];

    tiles[x][y] = nullopt;
    tiles[newX][y] = temp;

    return Ok;
}

Result TileGame::moveLeft(CoordinatePair coords) {
    if (!canMove(coords, Left)) return Fail;

    size_t x, y, newY;
    std::tie(x, newY) = coords;
    y = std::get<1>(coords);
    // go left a column
    newY -= 1;

    optional<int> temp = tiles[x][y];

    tiles[x][y] = nullopt;
    tiles[x][newY] = temp;

    return Ok;
}

Result TileGame::moveRight(CoordinatePair coords) {
    if (!canMove(coords, Right)) return Fail;

    size_t x, y, newY;
    std::tie(x, newY) = coords;
    y = std::get<1>(coords);
    // go right a column
    newY += 1;

    optional<int> temp = tiles[x][y];

    tiles[x][y] = nullopt;
    tiles[x][newY] = temp;

    return Ok;
}

bool TileGame::canMove(CoordinatePair coords, Direction direction) {
    switch (direction) {
        case Up: {
            int newX, y;
            std::tie(newX, y) = coords;
            std::get<0>(coords);
            // go up a row
            newX -= 1;

            if (newX < 0) {
                std::cout << "TileGame::canMove" << newX << " < 0: false; returning false" << std::endl;
                return false;
            }

            auto newCoords = CoordinatePair{newX, y};

            if (isOccupied(newCoords)) return false;
//
//            optional<int> temp = tiles[x][y];
//
//            tiles[x][y] = nullopt;
//            tiles[newX][y] = temp;
            break;
        }
        case Down: {
            int newX, y;
            std::tie(newX, y) = coords;
            std::get<0>(coords);
            // go down a row
            newX += 1;

            if (newX > ROWS - 1) return false;

            auto newCoords = CoordinatePair{newX, y};

            if (isOccupied(newCoords)) return false;
//
//            optional<int> temp = tiles[x][y];
//
//            tiles[x][y] = nullopt;
//            tiles[newX][y] = temp;
            break;
        }
        case Left: {
            int x, newY;
            std::tie(x, newY) = coords;
            std::get<1>(coords);
            // go left a column
            newY -= 1;

            if (newY < 0) return false;

            auto newCoords = CoordinatePair{x, newY};

            if (isOccupied(newCoords)) return false;


            break;
        }
        case Right: {
            int x, newY;
            std::tie(x, newY) = coords;
            std::get<1>(coords);
            // go right a column
            newY += 1;

            if (newY > COLUMNS - 1) return false;

            auto newCoords = CoordinatePair{x, newY};

            if (isOccupied(newCoords)) return false;
            break;
        }
    }

    return true;
}

bool TileGame::isOccupied(CoordinatePair coord) {
    size_t x, y;
    std::tie(x, y) = coord;

    std::cout << "TileGame::isOccupied - tiles[" << x << "][" << y << "].has_value(): " << tiles[x][y].has_value() << std::endl;
    return tiles[x][y].has_value();
}

#ifndef NDEBUG

void TileGame::debugPrintTileSurface() {
    std::stringstream sStream;

    sStream << "+-------+-------+-------+\n";

    for (auto arr: tiles) {
        for (auto item: arr) {
            sStream << "|  ["
                    << (item.has_value() ? std::to_string(item.value()) : " ")
                    << "]  ";
        }
        sStream << "|\n";
        sStream << "+-------+-------+-------+\n";
    }

    std::cout << "DEBUG:\n" << sStream.str() << std::endl;
}

void TileGame::debugPrintTileArray() {
    int n = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << n << ": " << (tiles[i][j].has_value() ? std::to_string(tiles[i][j].value()) : "none")
                      << std::endl;
            n++;
        }
    }
}

#endif // #ifndef NDEBUG



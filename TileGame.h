//
// Created by Griffin Thompson on 10/27/22.
//

#ifndef INC_07L_CHALLENGE_TILE_GAME_TILEGAME_H
#define INC_07L_CHALLENGE_TILE_GAME_TILEGAME_H

#include <array>
#include <optional>
#include <tuple>

using std::array;
using std::optional;
using std::tuple;


enum Direction {
    Up = 0,
    Down,
    Left,
    Right
};

enum Result { Ok, Fail };

class TileGame {
public:
    typedef tuple<int, int> CoordinatePair;
    TileGame();
    Result moveTile(int, Direction);
    bool isSorted();

#ifndef NDEBUG

    void debugPrintTileSurface();

    void debugPrintTileArray();

#endif

private:
    static const size_t ROWS    = 3;
    static const size_t COLUMNS = 3;
    typedef array<array<optional<int>, COLUMNS>, ROWS> TileGrid;

    TileGrid tiles;

    // the tile surface is a 3x3 square
    // +-------+-------+-------+
    // |  [x]  |  [x]  |  [x]  |
    // |  [x]  |  [ ]  |  [x]  |
    // |  [x]  |  [x]  |  [x]  |
    // +-------+-------+-------+
    // Where 'x' is a number 1-8
    // The blank space can be anywhere.

    // a 2D array of ints that may or may not have a value ('Some val', or 'none')
    void generateTileSurface();

    CoordinatePair where(optional<int>);

    Result moveUp(CoordinatePair);

    Result moveDown(CoordinatePair);

    Result moveLeft(CoordinatePair);

    Result moveRight(CoordinatePair);

    bool canMove(CoordinatePair, Direction);

    bool isOccupied(CoordinatePair);
};

#endif //INC_07L_CHALLENGE_TILE_GAME_TILEGAME_H

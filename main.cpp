//
// Created by Griffin Thompson on 10/27/22.
//
#include<iostream>

#include "TileGame.h"

int main() {
    auto game = TileGame();

    game.debugPrintTileSurface();
    game.debugPrintTileArray();
    while (true) {
        std::cout << "what num you wanna move" << std::endl;
        int numToMove;
        std::cin >> numToMove;

        std::cout << "up: 1\ndown: 2\nleft: 3\nright: 4" << std::endl;

        int ans;
        std::cin >> ans;

        if(ans == 1)
            game.moveTile(numToMove, Up);
        else if(ans == 2)
            game.moveTile(numToMove, Down);
        else if(ans == 3)
            game.moveTile(numToMove, Left);
        else if(ans == 4)
            game.moveTile(numToMove, Right);
        else if (ans == 5) {
            std::cout << "where do you wanna put " << numToMove << std::endl;
            std::cout << "row: ";
            int row;
            std::cin >> row;
            std::cout << "column: ";
            int column;
            std::cin >> column;
        }
        game.debugPrintTileSurface();

        if (game.isSorted())
            break;
    }
    std::cout << "you win chap." << std::endl;
    return 0;
}
//
// Created by viking on 23.04.20.
//

#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

#include "Tile.h"
#include "Boot.h"

class Game{

    Tile field[3][3]; // (x, y)
    int windowSize[2] = {1280, 720};

public:
    Game(){
        int width = windowSize[0] / 3;
        int height = windowSize[1] / 3;

        for (int y = 0; y < 3; y++){
            for (int x = 0; x < 3; x++){
                field[x][y].create(x * width, y * height, width, height);
            }
        }

    }

    ~Game() = default;

    Mark judge();

    void start();
};


#endif //TICTACTOE_GAME_H

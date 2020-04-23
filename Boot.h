//
// Created by viking on 23.04.20.
//

#ifndef TICTACTOE_BOOT_H
#define TICTACTOE_BOOT_H

#include <experimental/random>
#include "Tile.h"
#include "Player.h"

//#define SHOW_POSSIBLE_WINS
//#define SHOW_SCORES

class Boot :public Player{

public:
    Boot() = default;

    explicit Boot(Mark marker): Player(marker){}

    virtual inline sf::Vector2i makeMove(){}

};

class randomBoot :public Boot{

public:
    randomBoot(Mark marker) :Boot(marker){}

    inline sf::Vector2i makeMove() override{
        return {std::experimental::randint(0, 2), std::experimental::randint(0, 2)};
    }
};

class minimaxBoot :public Boot{

    int level;
    Mark testJudge(Mark field[3][3]);

    int minimax(Mark tile[3][3], Mark currSide, sf::Vector2i prev, int deep);

public:
    explicit minimaxBoot(Mark marker, short lvl = 9): Boot(marker), level(lvl){}

    sf::Vector2i makeMove(Tile field[3][3]);
};


#endif //TICTACTOE_BOOT_H

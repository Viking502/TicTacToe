//
// Created by viking on 23.04.20.
//

#ifndef TICTACTOE_PLAYER_H
#define TICTACTOE_PLAYER_H

class Player{

protected:
    Mark side;

public:
    Player(Mark pSide): side(pSide){}

    Player() = default;

    void setSide(Mark mark){
        side = mark;
    }

    Mark getSide(){
        return side;
    }
};

#endif //TICTACTOE_PLAYER_H

//
// Created by viking on 23.04.20.
//

#include "Boot.h"

Mark minimaxBoot::testJudge(Mark field[3][3]){

    Mark winner = Mark::UNSET;
    //check horizontal fields
    for(int y = 0; y < 3 && winner == Mark::UNSET; y++){
        winner = field[0][y];
        for(int x = 1; x < 3; x++) {
            if (field[x][y] != field[x - 1][y]) {
                winner = Mark::UNSET;
            }
        }
    }
    //check vertical fields
    for(int x = 0; x < 3 && winner == Mark::UNSET; x++){
        winner = field[x][0];
        for(int y = 1; y < 3; y++) {
            if (field[x][y] != field[x][y - 1]) {
                winner = Mark::UNSET;
            }
        }
    }
    //check diagonal fields
    if(winner == Mark::UNSET) {
        winner = field[0][0];
        for (int i = 1; i < 3; i++) {
            if (field[i][i] != field[i - 1][i - 1]) {
                winner = Mark::UNSET;
            }
        }
    }
    if(winner == Mark::UNSET) {
        winner = field[1][1];
        for (int i : {-1, 1}) {
            if (field[1][1] != field[1 + i][1 - i]) {
                winner = Mark::UNSET;
            }
        }
    }
    return winner;
}

int minimaxBoot::minimax(Mark tile[3][3], Mark currSide, sf::Vector2i prev, int deep){

    if(deep > level && level != -1){
        return 20;
    }

    Mark newTile[3][3];

    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            newTile[x][y] = tile[x][y];
        }
    }
    newTile[prev.x][prev.y] = currSide;
    Mark winner = testJudge(newTile);
    if(winner != UNSET){

#ifdef SHOW_POSSIBLE_WINS
        std::cout <<"== winner: " << (winner == Mark::O ? "O" : "X")  << " in: " << deep << " ==\n";
            char temp;
            for(int y = 0; y < 3; y++){
                for(int x = 0; x < 3; x++){
                    if(newTile[x][y] == Mark::O){
                        temp = 'O';
                    }else if(newTile[x][y] == Mark::X){
                        temp = 'X';
                    }else{
                        temp = '_';
                    }
                    std::cout << temp << " ";
                }
                std::cout << "\n\n";
            }
#endif

        return (winner == Mark::O ? deep : 100);
    }

    currSide = (currSide == Mark::O ? Mark::X : Mark::O);
    int points;
    int bestPoints;

    if(level != -1) {
        bestPoints = (currSide == Mark::O ? 101 : -1);
    }else{
        bestPoints = (currSide == Mark::O ? -1 : 101);
    }


    for(int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {

            if(newTile[x][y] == Mark::UNSET) {

                points = minimax(newTile, currSide, sf::Vector2i(x, y), deep + 1);
                if ((currSide == Mark::O && level != -1)
                    || (currSide == Mark::X && level == -1)) {
                    bestPoints = std::min(bestPoints, points);
                } else {
                    bestPoints = std::max(bestPoints, points);
                }
            }

        }
    }
    return (bestPoints == -1 || bestPoints == 101 ? 50 : bestPoints);
}

sf::Vector2i minimaxBoot::makeMove(Tile field[3][3]){

    Mark tile[3][3];

    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            tile[x][y] = field[x][y].getState();
        }
    }

    sf::Vector2i bestMove;
    int maxPoints = (level == -1 ? 101 : -1);
    int points;

    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){

            if(tile[x][y] == Mark::UNSET){
                points = minimax(tile, side, sf::Vector2i(x, y), 1);

                if((points > maxPoints && level != -1) ||
                   (points < maxPoints && level == -1)) {
                    maxPoints = points;
                    bestMove.x = x;
                    bestMove.y = y;
                }else if(points == maxPoints && std::experimental::randint(0, 2 * (10 - (x + 1 + 3 * y))) < (10 - (x + 2 + 3 * y))){
                    bestMove.x = x;
                    bestMove.y = y;
                }

#ifdef SHOW_SCORES
                std::cout << "(" << x << ", " << y << ") -> " << points << "\n";
#endif
            }
        }
    }
#ifdef SHOW_SCORES
    std::cout << "best score: " << maxPoints << "\n";
#endif
    return bestMove;
}

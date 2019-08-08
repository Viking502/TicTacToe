#include <iostream>
#include <SFML/Graphics.hpp>
#include <experimental/random>
#import <algorithm>

//#define SHOW_POSSIBLE_WINS
#define SHOW_SCORES

enum Mark{
    UNSET = 0,
    O = 1,
    X = 2
};

class Game;

class Tile{

    int posX;
    int posY;

    int width;
    int height;

    Mark state = UNSET; // 'x', 'o', '-'

    sf::Color color = sf::Color(120, 120, 120);

    sf::RectangleShape shape;
    sf::CircleShape* mark = nullptr;

public:
    Tile() = default;

    ~Tile(){
        delete mark;
        mark = nullptr;
    }

    void create(int x, int y, int dx, int dy) {

        posX = x;
        posY = y;

        width = dx;
        height = dy;

        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(dx, dy));
        shape.setFillColor(sf::Color(120, 120, 120));
        shape.setOutlineColor(sf::Color(220, 220, 220));
        shape.setOutlineThickness(4);
        //shape.setOrigin(0, 0);
        state = UNSET;
    }

    bool lightUp(sf::Vector2i mousePos){
        if((mousePos.x > posX && mousePos.x < posX + width)
           &&(mousePos.y > posY && mousePos.y < posY + height)
           && (state == UNSET)){

            shape.setFillColor(color + sf::Color(40, 40, 40));
            return true;
        }
        shape.setFillColor(color);
        return false;
    }

    bool checkClick(sf::Vector2i mousePos, Mark playerMark){

        if((mousePos.x > posX && mousePos.x < posX + width)
            && (mousePos.y > posY && mousePos.y < posY + height)){

            return validMove(playerMark);
        }
        return false;
    }

    bool validMove(Mark playerMark){
        if(state == Mark::UNSET){
            setState(playerMark);
            return true;
        }
        return false;
    }

    void setState(Mark newState){

        state = newState;

        mark = new sf::CircleShape(height / 2.4);

        mark -> setOrigin(height / 2, height / 2.4);
        mark -> setPosition(posX + width / 2, posY + height / 2);
        mark -> setFillColor(sf::Color(120, 120, 120));

        if(state == Mark::O) {
            mark -> setOutlineThickness(12);
            mark -> setOutlineColor(sf::Color(60, 210, 60));
        }else if(state == Mark::X){
            mark -> setOutlineThickness(12);
            mark -> setOutlineColor(sf::Color(210, 60, 60));
        }
    }

    Mark getState(){
        return state;
    }
    
    void draw(sf::RenderWindow* win){
        win -> draw(shape);

        if(mark != nullptr){
            win -> draw(*mark);
        }
    }

};

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

class Boot :public Player{

public:
    Boot() = default;

    Boot(Mark marker): Player(marker){}

    virtual inline sf::Vector2i makeMove(){}

};

class randomBoot :public Boot{

public:
    randomBoot(Mark marker) :Boot(marker){}

    inline sf::Vector2i makeMove(){
        return sf::Vector2i(std::experimental::randint(0, 2), std::experimental::randint(0, 2));
    }
};

class minimaxBoot :public Boot{

    int level;
    Mark testJudge(Mark field[3][3]){

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

    int minimax(Mark tile[3][3], Mark currSide, sf::Vector2i prev, int deep){

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

public:
    minimaxBoot(Mark marker, short lvl = 9): Boot(marker), level(lvl){}

    sf::Vector2i makeMove(Tile field[3][3]){

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
};


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

    Mark judge(){

        Mark winner = Mark::UNSET;
        //check horizontal fields
        for(int y = 0; y < 3 && winner == Mark::UNSET; y++){
            winner = field[0][y].getState();
            for(int x = 1; x < 3; x++) {
                if (field[x][y].getState() != field[x - 1][y].getState()) {
                    winner = Mark::UNSET;
                }
            }
        }
        //check vertical fields
        for(int x = 0; x < 3 && winner == Mark::UNSET; x++){
            winner = field[x][0].getState();
            for(int y = 1; y < 3; y++) {
                if (field[x][y].getState() != field[x][y - 1].getState()) {
                    winner = Mark::UNSET;
                }
            }
        }
        //check diagonal fields
        if(winner == Mark::UNSET) {
            winner = field[0][0].getState();
            for (int i = 1; i < 3; i++) {
                if (field[i][i].getState() != field[i - 1][i - 1].getState()) {
                    winner = Mark::UNSET;
                }
            }
        }
        if(winner == Mark::UNSET) {
            winner = field[1][1].getState();
            for (int i : {-1, 1}) {
                if (field[1][1].getState() != field[1 + i][1 - i].getState()) {
                    winner = Mark::UNSET;
                }
            }
        }
        return winner;
    }

    void start(){

        sf::RenderWindow window(sf::VideoMode(windowSize[0], windowSize[1]), "tic tac toe");

        Player pOne(Mark::O);
        //Player pTwo(Mark::X);
        minimaxBoot pTwo(Mark::X, 9);

        Player* pHandle = (std::experimental::randint(0, 1) == 1 ? &pOne : &pTwo);
        //Player* pHandle = &pTwo; // boot starts


        Mark winner = Mark::UNSET;
        short tourCounter = 0;

        while(window.isOpen()){

            sf::Event event;

            // boot move
            if(pHandle == &pTwo){

                bool movedFlag = false;
                do {
                    sf::Vector2i move = pTwo.makeMove(field);
                    movedFlag = field[move.x][move.y].validMove(pTwo.getSide());
                }while(!movedFlag);

                ++tourCounter;
                pHandle = (pHandle == &pOne ? &pTwo : &pOne);
                winner = judge();

            }

            while(window.pollEvent(event)){
                if((event.type == sf::Event::Closed) ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)){
                    window.close();
                }
                //if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left){
                if(event.type == sf::Event::MouseButtonPressed){

                    if(event.mouseButton.button == sf::Mouse::Left){
                        for (int y = 0; y < 3; y++) {
                            for (int x = 0; x < 3; x++) {

                                if(field[x][y].checkClick(sf::Mouse::getPosition(window), pHandle -> getSide())){
                                    //change player
                                    ++tourCounter;
                                    pHandle = (pHandle == &pOne ? &pTwo : &pOne);
                                    winner = judge();
                                    break;
                                }
                            }
                        }
                    }
                }
                if(event.type == sf::Event::MouseMoved){
                    for (int y = 0; y < 3; y++) {
                        for (int x = 0; x < 3; x++) {
                            if(field[x][y].lightUp(sf::Mouse::getPosition(window))){

                                field[x][y].draw(&window);
                                window.display();
                                break;
                            }
                        }
                    }
                }
            }
            //draw scene
            window.clear();
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    field[x][y].draw(&window);
                }
            }
            window.display();

            if(winner != Mark::UNSET || tourCounter >= 9){
                char temp;
                if(winner == Mark::O){
                    std::cout << "=== O wins ===\n";
                }else if(winner == Mark::X){
                    std::cout << "=== X wins ===\n";
                }else{
                    std::cout << "___ Draw ___\n";
                }

                window.close();
            }
        }
    }
};

int main() {

    Game* newGame = new Game();

    newGame -> start();

    delete newGame;

    return 0;
}
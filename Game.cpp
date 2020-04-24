//
// Created by viking on 23.04.20.
//

#include <experimental/random>
#include "Game.h"
#include <iostream>

Mark Game::judge(){

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

void Game::start(){

    sf::RenderWindow window(sf::VideoMode(windowSize[0], windowSize[1]), "tic tac toe");

    Player pOne(Mark::O);
    //Player pTwo(Mark::X);
    minimaxBoot pTwo(Mark::X, 9);

    Player* pHandle = (std::experimental::randint(0, 1) == 1 ? &pOne : &pTwo);
    //Player* pHandle = &pTwo; // boot starts


    Mark winner = Mark::UNSET;
    short tourCounter = 0;
    sf::Event event;

    while(window.isOpen()){

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

            std::string translate[3] = {"Draw!  ", "O wins!", "X wins!"};

            sf::Font font{};
            if(font.loadFromFile("../rushbrush.ttf")){
                sf::Text temp{};
                temp.setFont(font);
                temp.setPosition(windowSize[0] / 4, windowSize[1] / 4);
                temp.setCharacterSize(260);
                temp.setFillColor(sf::Color::Yellow);
                temp.setString(translate[winner]);

                window.draw(temp);
                window.display();
            }else{
                std::cout << "===============\n";
                std::cout << "=== " << translate[winner] << " ===" << "\n";
                std::cout << "===============\n";
            }
            sf::sleep(sf::seconds(1));
            window.close();
        }
    }
}
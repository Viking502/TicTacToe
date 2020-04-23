//
// Created by viking on 23.04.20.
//

#ifndef TICTACTOE_TILE_H
#define TICTACTOE_TILE_H

#include <SFML/Graphics.hpp>

enum Mark{
    UNSET = 0,
    O = 1,
    X = 2
};

class Tile{

    int posX = 0;
    int posY = 0;

    int width = 0;
    int height = 0;

    Mark state = UNSET; // 'x', 'o', '-'

    sf::Color color = sf::Color(120, 120, 120);

    sf::RectangleShape shape;
    sf::Shape* mark = nullptr;

public:
    Tile() = default;

    ~Tile(){
        delete mark;
        mark = nullptr;
    }

    void create(int x, int y, int dx, int dy);

    bool lightUp(sf::Vector2i mousePos);

    bool checkClick(sf::Vector2i mousePos, Mark playerMark);

    bool validMove(Mark playerMark);

    void setState(Mark newState);

    void draw(sf::RenderWindow* win);

    Mark getState(){
        return state;
    }
};


#endif //TICTACTOE_TILE_H

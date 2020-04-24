//
// Created by viking on 23.04.20.
//

#include "Tile.h"

void Tile::create(int x, int y, int dx, int dy) {

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

bool Tile::lightUp(sf::Vector2i mousePos){
    if((mousePos.x > posX && mousePos.x < posX + width)
       &&(mousePos.y > posY && mousePos.y < posY + height)
       && (state == UNSET)){

        shape.setFillColor(color + sf::Color(40, 40, 40));
        return true;
    }
    shape.setFillColor(color);
    return false;
}

bool Tile::checkClick(sf::Vector2i mousePos, Mark playerMark){

    if((mousePos.x > posX && mousePos.x < posX + width)
       && (mousePos.y > posY && mousePos.y < posY + height)){

        return validMove(playerMark);
    }
    return false;
}

bool Tile::validMove(Mark playerMark){
    if(state == Mark::UNSET){
        setState(playerMark);
        return true;
    }
    return false;
}

void Tile::setState(Mark newState){

    state = newState;

    mark.setPosition(posX + width / 3, posY + height / 8);
    if(state == Mark::O) {
        mark.setString("O");
        mark.setFillColor(sf::Color::Green);
    }else if(state == Mark::X){
        mark.setString("X");
        mark.setFillColor(sf::Color::Red);
    }
}

void Tile::draw(sf::RenderWindow* win){
    win -> draw(shape);

    if(state != Mark::UNSET){
        win -> draw(mark);
    }
}
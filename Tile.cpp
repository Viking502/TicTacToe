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

void Tile::draw(sf::RenderWindow* win){
    win -> draw(shape);

    if(mark != nullptr){
        win -> draw(*mark);
    }
}
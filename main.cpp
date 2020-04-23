#include <iostream>
#include <SFML/Graphics.hpp>
#include <experimental/random>
#import <algorithm>
#import "Tile.h"
#import "Player.h"
#import "Boot.h"
#include "Game.h"


int main() {

    Game* newGame = new Game();

    newGame -> start();

    delete newGame;

    return 0;
}
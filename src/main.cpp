// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "core/GameEngine.h"

int main()
{
    GameEngine game("../bin/config/config.json");
    game.run();
}


//
// Created by Matthew Crossan on 10/6/23.
//

#ifndef LIGHTCASTING_PLAYERSTATESYSTEM_H
#define LIGHTCASTING_PLAYERSTATESYSTEM_H


#include "../include/System.h"

#include "../components/Components.h"

class PlayerStateSystem : public System{
public:
    PlayerStateSystem();
    void update();
};


#endif //LIGHTCASTING_PLAYERSTATESYSTEM_H

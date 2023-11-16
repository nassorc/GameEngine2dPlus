//
// Created by Matthew Crossan on 10/1/23.
//

#ifndef LIGHTCASTING_PLAYERINPUTSYSTEM_H
#define LIGHTCASTING_PLAYERINPUTSYSTEM_H

#include "../include/System.h"

class PlayerInputSystem : public System {
public:
    PlayerInputSystem();
    void update() override;
    void wallJump();
};


#endif //LIGHTCASTING_PLAYERINPUTSYSTEM_H

//
// Created by Matthew Crossan on 10/2/23.
//

#ifndef LIGHTCASTING_MOVEMENTSYSTEM_H
#define LIGHTCASTING_MOVEMENTSYSTEM_H

#include "../include/System.h"

class MovementSystem : public System {
public:
    MovementSystem();
    void update() override;
};


#endif //LIGHTCASTING_MOVEMENTSYSTEM_H

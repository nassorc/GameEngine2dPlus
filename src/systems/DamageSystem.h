//
// Created by Matthew Crossan on 10/6/23.
//

#ifndef LIGHTCASTING_DAMAGESYSTEM_H
#define LIGHTCASTING_DAMAGESYSTEM_H

#include "../include/System.h"

class DamageSystem : public System {
public:
    DamageSystem();
    void update() override;
    void updateHitboxPosition();
};


#endif //LIGHTCASTING_DAMAGESYSTEM_H

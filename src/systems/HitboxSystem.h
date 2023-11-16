//
// Created by Matthew Crossan on 10/6/23.
//

#ifndef LIGHTCASTING_HITBOXSYSTEM_H
#define LIGHTCASTING_HITBOXSYSTEM_H

#include "../include/System.h"

#include "../components/Components.h"

class CoreManager;

class HitboxSystem : public System {
public:
    HitboxSystem();
    void update() override;
    void hitboxFollowSource();
    void hitboxStaticPosition();
};

#endif //LIGHTCASTING_HITBOXSYSTEM_H

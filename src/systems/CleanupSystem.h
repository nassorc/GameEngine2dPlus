//
// Created by Matthew Crossan on 10/4/23.
//

#ifndef LIGHTCASTING_CLEANUPSYSTEM_H
#define LIGHTCASTING_CLEANUPSYSTEM_H

#include "../include/System.h"

class CleanupSystem : public System{
public:
    CleanupSystem();
    void update() override;

};


#endif //LIGHTCASTING_CLEANUPSYSTEM_H

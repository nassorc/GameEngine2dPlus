//
// Created by Matthew Crossan on 10/6/23.
//

#ifndef LIGHTCASTING_LIFESPANSYSTEM_H
#define LIGHTCASTING_LIFESPANSYSTEM_H

#include "../include/System.h"

class LifespanSystem : public System{
public:
    LifespanSystem();
    void update() override;
};


#endif //LIGHTCASTING_LIFESPANSYSTEM_H

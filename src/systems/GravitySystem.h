//
// Created by Matthew Crossan on 10/3/23.
//

#ifndef LIGHTCASTING_GRAVITYSYSTEM_H
#define LIGHTCASTING_GRAVITYSYSTEM_H

#include "../include/System.h"
#include "../core/Types.h"

class GravitySystem : public System {
public:
    GravitySystem();
    void update() override;
};


#endif //LIGHTCASTING_GRAVITYSYSTEM_H

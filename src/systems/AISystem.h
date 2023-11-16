//
// Created by Matthew Crossan on 10/7/23.
//

#ifndef LIGHTCASTING_AISYSTEM_H
#define LIGHTCASTING_AISYSTEM_H

#include "../include/System.h"

class AISystem : public System {
public:
    AISystem();
    void update() override;
};


#endif //LIGHTCASTING_AISYSTEM_H

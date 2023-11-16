//
// Created by Matthew Crossan on 10/2/23.
//

#ifndef LIGHTCASTING_PHYSICSSYSTEM_H
#define LIGHTCASTING_PHYSICSSYSTEM_H

#include <map>
#include "../include/System.h"

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    void update() override;
    void collisionDetection();
    void entityCollisionResolution();

};


#endif //LIGHTCASTING_PHYSICSSYSTEM_H

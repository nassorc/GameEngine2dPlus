//
// Created by Matthew Crossan on 10/3/23.
//

#ifndef LIGHTCASTING_RENDERSYSTEM_H
#define LIGHTCASTING_RENDERSYSTEM_H

#include "../include/System.h"
#include "../core/Types.h"

class RenderSystem : public System {
public:
    RenderSystem();
    void update() override;
};


#endif //LIGHTCASTING_RENDERSYSTEM_H

//
// Created by Matthew Crossan on 10/13/23.
//

#ifndef LIGHTCASTING_DIALOGUESYSTEM_H
#define LIGHTCASTING_DIALOGUESYSTEM_H


#include "../include/System.h"

class DialogueSystem : public System {
public:
    DialogueSystem();
    void update() override;
    void openDialogue(const std::string& config, const std::string& name);
};


#endif //LIGHTCASTING_DIALOGUESYSTEM_H

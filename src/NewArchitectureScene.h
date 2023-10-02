//
// Created by Matthew Crossan on 10/1/23.
//

#ifndef LIGHTCASTING_NEWARCHITECTURESCENE_H
#define LIGHTCASTING_NEWARCHITECTURESCENE_H

#include "Scene.h"

class NewArchitectureScene : public Scene {
private:
public:
    NewArchitectureScene(GameEngine *gameEngine, Assets &assetManager);

    void update() override;
    void sDoAction(const Action &action) override;
    void sRender() override;

    void onEnd() override;

};


#endif //LIGHTCASTING_NEWARCHITECTURESCENE_H

//
// Created by Matthew Crossan on 9/23/23.
//

#pragma once

#include "../scenes/Scene.h"
#include "../scenes/SceneBattle.h"
#include "nlohmann/json.hpp"
#include <utility>
using json = nlohmann::json;

class ScenePlay;

class LevelLoader1 {
protected:
public:
    LevelLoader1() {}
    void loader(Scene* scene, json data) {
        if(auto sceneLevel = dynamic_cast<SceneBattle*>(scene)) {
            strategy(sceneLevel, std::move(data));
        }
    }
    virtual void strategy(SceneBattle* level, json data) = 0;
};

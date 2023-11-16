//
// Created by Matthew Crossan on 9/23/23.
//

#pragma once

#include "../../scenes/Scene.h"
#include "../../(deprecated)/ScenePlay.h"
#include "nlohmann/json.hpp"
#include <utility>
using json = nlohmann::json;

class ScenePlay;

class LevelLoader {
protected:
public:
    LevelLoader() {}
    void loader(Scene* scene, json data) {
        if(auto sceneLevel = dynamic_cast<ScenePlay*>(scene)) {
            strategy(sceneLevel, std::move(data));
        }
    }
    virtual void strategy(ScenePlay* level, json data) = 0;
};

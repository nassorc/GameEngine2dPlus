//
// Created by Matthew Crossan on 9/24/23.
//

#pragma once

#include "LevelLoader.h"

class WorldObjectsLevelLoader : public LevelLoader {
public:
    WorldObjectsLevelLoader() {}
    void strategy(ScenePlay* level, json layer) override {
        if(layer["name"] == "playerspawn") {
            json object = layer["objects"][0];
            int spawnX = ((int) object["x"])*2.667;
            int spawnY = ((int)object["y"])*2.667;

            level->m_playerConfig.S = 6;
            level->m_playerConfig.ML = 5;
            level->m_playerConfig.BX = 64;
            level->m_playerConfig.BY = 64;

            level->m_playerConfig.X = spawnX;
            level->m_playerConfig.Y = spawnY;
        }
        if(layer["name"] == "shop") {
            json object = layer["objects"][0];
            auto& animation = level->m_assetManager.getAnimation("Shop");
            auto animationSize = animation.getSprite().getLocalBounds().getSize();
            auto spriteScale = animation.getSprite().getScale();

//                createEntity()
//                    .add<CTransform>()
//                    .add<>

            int x = (float) object["x"] * 2.667;
            int y = (float) object["y"] * 2.667 - ((animationSize.y * spriteScale.y) / 2);
            auto e = level->m_entityManager.addEntity("shop");
            e->add<CTransform>(Vec2{(float) x, (float) y}, Vec2{0, 0}, 0)
                    .add<CAnimation>(animation)
                    .add<CBoundingBox>(Vec2{animationSize.x * spriteScale.x , animationSize.y * spriteScale.y}, false, false)
                    .add<CDialogue>();
//                e->addComponent<CTransform>(Vec2{(float) x, (float) y}, Vec2{0, 0}, 0);
//                e->addComponent<CAnimation>(animation);
//                e->addComponent<CBoundingBox>(Vec2{animationSize.x * spriteScale.x , animationSize.y * spriteScale.y}, false, false);
//                e->addComponent<CDialogue>();
        }
        if(layer["name"] == "enemyspawn") {
            json objects = layer["objects"];
            for(auto object : objects) {
                auto& animation = level->m_assetManager.getAnimation("SlimeRun");
                auto animationSize = animation.getSprite().getLocalBounds().getSize();
                auto spriteScale = animation.getSprite().getScale();

                int x = (float) object["x"] * 2.667;
                int y = (float) object["y"] * 2.667 - ((animationSize.y * spriteScale.y) / 2);
                auto e = level->m_entityManager.addEntity("enemy");
                e->addComponent<CTransform>(Vec2{(float) x, (float) y}, Vec2{2, 0}, 0);
                e->addComponent<CAnimation>(animation);
                e->addComponent<CBoundingBox>(Vec2{animationSize.x, animationSize.y}, false, false);
                e->addComponent<CDamage>(2);

            }
        }
        if(layer["name"] == "enemyboundary") {
            json objects = layer["objects"];
            for(auto object : objects) {
                int x = (float) object["x"] * 2.667;
                int y = (float) object["y"] * 2.667;
                auto e = level->m_entityManager.addEntity("enemyboundary");
                e->addComponent<CTransform>(Vec2{(float) x + 32, (float) y + 32}, Vec2{0, 0}, 0);
                e->addComponent<CBoundingBox>(Vec2{(float) level->m_levelConfig.TILE_WIDTH, (float) level->m_levelConfig.TILE_WIDTH}, false, false);
            }
        }

    }

};


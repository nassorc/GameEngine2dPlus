//
// Created by Matthew Crossan on 9/24/23.
//

#pragma once

#include "LevelLoader1.h"

extern World world;

class TilesetLevelLoader1 : public LevelLoader1 {
public:
    TilesetLevelLoader1() {}

    void strategy(SceneBattle* level, json layer) override {
        std::string textureName = layer["name"];
        std::vector<int> tilemap = layer["data"];
        int height = layer["height"];
        int width = layer["width"];

        int row = 0;
        int col = 0;

        // loop through each tile
        for (size_t tile=0; tile<height*width; tile++) {
            if(textureName == "terrain") {
//                    if(tilemap[tile] == 0) { continue; }
                if(tile % 30 == 0 && tile != 0) {
                    row++;
                    col = 0;
                }
                if(tilemap[tile] != 0) {

                    auto tileEntity = world.createEntity("terrain");
                    world.set<CTransform>(tileEntity, Vec2(col * 64 + 32, row * 64 + 32), Vec2(0, 0), 0.f);
                    world.set<CBoundingBox>(tileEntity, Vec2(64, 64), true, true );
                    world.set<CAnimation>(tileEntity, level->m_assetManager.getAnimation(std::to_string(tilemap[tile] - 1)) );
                    world.set<CAnimation>(tileEntity, level->m_assetManager.getAnimation(std::to_string(tilemap[tile] - 1)) );



//                    auto e = level->m_entityManager.addEntity("terrain");
//                    e->addComponent<CTransform>(Vec2(col*64 + 32,row*64 + 32), Vec2(0,0), 0);
//                    e->addComponent<CBoundingBox>(Vec2(64, 64), true, true);
//                    auto& eAnimation = e->addComponent<CAnimation>(level->m_assetManager.getAnimation(std::to_string(tilemap[tile]-1)));
                }
                col++;
            }
        }
    }
};



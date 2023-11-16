//
// Created by Matthew Crossan on 10/6/23.
//

#ifndef LIGHTCASTING_TILEDPARSER1_H
#define LIGHTCASTING_TILEDPARSER1_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Tileset {
private:
    std::vector<int> m_tileset;
    int height, width;
public:
    Tileset() {}
    int getData(int x, int y);
};

class TiledParser1 {
public:
    TiledParser1(std::string config);
    void getLayers();
    void getObjects();
    void getImages();
    void getTilesets();
    void getObjectByName();
    void getImageByName();
    void getTilesetByName();
};


#endif //LIGHTCASTING_TILEDPARSER1_H

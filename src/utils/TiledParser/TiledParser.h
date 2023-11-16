//
// Created by Matthew Crossan on 10/16/23.
//

#ifndef LIGHTCASTING_TILEDPARSER_H
#define LIGHTCASTING_TILEDPARSER_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;
//
//class ILayer {
//private:
//    std::string name;
//
//public:
//    ILayer();
//};

// Layer
//   Object
//   Tileset
//   Image


class TiledParser {
private:
public:
    TiledParser();
    void getTilesetByName(std::string name);
    void getObjectByName(std::string name);
};


#endif //LIGHTCASTING_TILEDPARSER_H

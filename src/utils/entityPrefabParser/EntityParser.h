//
// Created by Matthew Crossan on 10/12/23.
//

#ifndef LIGHTCASTING_ENTITYPARSER_H
#define LIGHTCASTING_ENTITYPARSER_H

#include <nlohmann/json.hpp>
#include <string>
#include "ComponentParserRegistry.h"

using json = nlohmann::json;

const std::string K_ENTITY_COMPONENTS = "components";
const std::string K_COMPONENT         = "component";
const std::string K_DATA              = "data";

class EntityParser {
public:
    EntityParser() {}
    void parseFromJSON(json entities);
};


#endif //LIGHTCASTING_ENTITYPARSER_H

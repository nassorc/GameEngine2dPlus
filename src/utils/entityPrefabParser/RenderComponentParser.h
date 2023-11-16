//
// Created by Matthew Crossan on 10/11/23.
//

#ifndef LIGHTCASTING_RENDERCOMPONENTPARSER_H
#define LIGHTCASTING_RENDERCOMPONENTPARSER_H

#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include "ComponentParserRegistry.h"
#include "../../components/Components.h"

using json = nlohmann::json;

const std::string K_WIDTH = "width";
const std::string K_HEIGHT = "height";
const std::string K_FILLCOLOR = "fill_color";
const std::string K_OUTLINECOLOR = "outline_color";

//class IComponentParser {
//public:
//    IComponentParser() {}
//    virtual void parse() {}
//};

class RenderComponentParser {
public:
    RenderComponentParser() {
        ComponentParserRegistry::getInstance()->parserRegistry["render"] = std::bind(&RenderComponentParser::parse, this, std::placeholders::_1);
    }
    std::pair<std::string, Component*> parse(json componentData) {
        float width = componentData[K_WIDTH];
        float height = componentData[K_HEIGHT];

        std::array<uint8_t , 3> fc;
        std::array<int, 3> oc;
        for(size_t i = 0; i < 3; i++) {
            fc[i] = componentData[K_FILLCOLOR][i];
            oc[i] = componentData[K_OUTLINECOLOR][i];
        }
        auto rect = new CRect(width, height);
        rect->rect.setFillColor(sf::Color(fc[0], fc[1], fc[2]));

        std::cout << "setting fill to " << (int) rect->rect.getFillColor().r << " " << (int) rect->rect.getFillColor().g << " " << (int) rect->rect.getFillColor().b << std::endl;

        std::string typeName = typeid(CRect).name();
        return std::make_pair(typeName, std::move(rect));
//        componentData[K_HEIGHT];
//        componentData[K_FILLCOLOR];
//        componentData[K_OUTLINECOLOR];
    }
};


#endif //LIGHTCASTING_RENDERCOMPONENTPARSER_H

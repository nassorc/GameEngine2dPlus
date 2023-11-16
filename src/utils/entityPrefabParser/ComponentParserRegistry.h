//
// Created by Matthew Crossan on 10/11/23.
//

#ifndef LIGHTCASTING_COMPONENTPARSERREGISTRY_H
#define LIGHTCASTING_COMPONENTPARSERREGISTRY_H

#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include "../../components/Components.h"

using json = nlohmann::json;

typedef std::function<std::pair<std::string,Component*>(json)> ParseFnType;

class ComponentParserRegistry {
private:
    ComponentParserRegistry() = default;
    ComponentParserRegistry(ComponentParserRegistry& other) = delete;
    ComponentParserRegistry& operator= (ComponentParserRegistry& other) = delete;

    static ComponentParserRegistry* m_instance;

public:
    static ComponentParserRegistry* getInstance() {
        if(m_instance == nullptr) {
            m_instance = new ComponentParserRegistry();
        }
        return m_instance;
    }

    std::map<std::string, ParseFnType> parserRegistry;
};

#endif //LIGHTCASTING_COMPONENTPARSERREGISTRY_H

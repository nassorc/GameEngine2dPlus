//
// Created by Matthew Crossan on 10/12/23.
//

#include "EntityParser.h"
#include "../../core/World.h"

extern World world;

// BUG:
// traced the function calls. The data seems fine before `insertData`, but when
// we query the entity and extract its component data is prints the wrong data.

// we are adding the entity, so the problem may be from adding the entity in the components array

void EntityParser::parseFromJSON(json entities) {
    int counter = 0;
    for(auto entity : entities) {
        counter++;
        // create entity
        // loop through each component
        auto e = world.createEntity("prefab");
        for(auto component : entity[K_ENTITY_COMPONENTS]) {
            // map parser name to component array
            // create component
            // check if component parser exists
            if(ComponentParserRegistry::getInstance()->parserRegistry.find(component[K_COMPONENT]) !=
               ComponentParserRegistry::getInstance()->parserRegistry.end())
            {
                auto p = ComponentParserRegistry::getInstance()->parserRegistry[component[K_COMPONENT]](component[K_DATA]);
//                auto rect = *(p.second);

//                std::cout << rect->rect.getSize().x << " " << rect->rect.getSize().y << std::endl;

//                world.coreManager->m_componentManager->addComponentByComponentId(p.first, e, *rect);
                world.coreManager->addComponentByTypeName(p.first, e, p.second);
            }
        }
    }
}

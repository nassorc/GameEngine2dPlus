//
// Created by Matthew Crossan on 10/1/23.
//

#ifndef LIGHTCASTING_ENTITYCOMPONENT_H
#define LIGHTCASTING_ENTITYCOMPONENT_H

#include "ComponentManager1.h"
#include "Components1.h"
//#include "Entity1.h"

class Entity1;

class EntityComponent {
private:
    std::shared_ptr<ComponentManager1> m_componentManager;
public:
    EntityComponent(std::shared_ptr<ComponentManager1> cm) {}

    template<class T>
    T getEntityComponent(std::shared_ptr<Entity1> e) {
        return m_componentManager->addComponent(e, Position{0,1});
    }
};
#include "EntityComponent.cpp"

#endif //LIGHTCASTING_ENTITYCOMPONENT_H

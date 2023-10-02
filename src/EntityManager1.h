//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "ComponentManager1.h"
#include "Entity1.h"

using namespace std;

class EntityManager1 {
private:
    size_t m_totalEntities = 0;
    size_t m_livingEntityCount = 0;
    vector<shared_ptr<Entity1>> m_entities={};
    std::shared_ptr<ComponentManager1> m_componentManager;

public:
    EntityManager1(std::shared_ptr<ComponentManager1> cm) { }

    shared_ptr<Entity1> createEntity(string tag = "default") {
        size_t nextEntity = m_totalEntities;
        auto entity = std::make_shared<Entity1>(nextEntity, tag, m_componentManager);
        m_entities.push_back(entity);

        m_livingEntityCount++;
        m_totalEntities++;
        return entity;
    }
    void destroyEntity(vector<Entity1> vec) {
        auto itr = std::remove_if(vec.begin(), vec.end(), [](const Entity1& e) {
            return !e.isActive();
        });

        vec.erase(itr, vec.end());
        m_livingEntityCount--;
        m_livingEntityCount--;
    }
    template<class T>
    T& getEntityComponent(Entity1& entity) {
        return m_componentManager->getComponent<T>(entity);
    }
};
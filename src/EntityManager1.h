//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Entity1.h"

using namespace std;

typedef std::vector<std::shared_ptr<Entity1>> Entity1Vector;
typedef std::map<std::string, Entity1Vector> Entity1Map;

class EntityManager1 {
private:
    Entity1Vector m_entities={};
    Entity1Vector m_entitiesToAdd;
    Entity1Map    m_entityMap;
    size_t        m_totalEntities = 0;
    size_t        m_livingEntityCount = 0;

public:
    EntityManager1() { }

    shared_ptr<Entity1> createEntity(string tag = "default") {
        size_t nextEntity = m_totalEntities;
        auto entity = std::make_shared<Entity1>(nextEntity, tag);
        m_entitiesToAdd.push_back(entity);

        m_livingEntityCount++;
        m_totalEntities++;
        return entity;
    }
    void update() {
        for (auto& e : m_entitiesToAdd)
        {
            m_entities.push_back(e);
            m_entityMap[e->tag()].push_back(e);
        }

        removeDeadEntities(m_entities);

        for (auto& em : m_entityMap)
        {
            removeDeadEntities(em.second);
        }
        m_entitiesToAdd.clear();
    }
    void removeDeadEntities(Entity1Vector& vec) {
        auto itr = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity1> e) {
            return !e->isActive();
        });

        vec.erase(itr, vec.end());
        m_livingEntityCount--;
        m_livingEntityCount--;
    }
    Entity1Vector getNewEntities() {
        return m_entitiesToAdd;
    }
};
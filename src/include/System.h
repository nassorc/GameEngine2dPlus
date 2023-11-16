//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once
#include <iostream>
#include <set>
#include <vector>
#include "../core/Entity1.h"

class SystemManager1;

class System {
    friend class SystemManager1;

protected:
    std::vector<shared_ptr<Entity1>> m_entities;
    std::vector<shared_ptr<Entity1>> m_entitiesToAdd;

public:
    System() = default;
    virtual void update() {
        for (auto& e : m_entitiesToAdd)
        {
            // check if entity is already in system's entity list
            auto itr = std::find_if(m_entities.begin(), m_entities.end(), [e](std::shared_ptr<Entity1> entityInSystem) {
                return entityInSystem->id() == e->id();
            });
            // if entity not found, add new entity to list
            if(itr == m_entities.end()) {
                m_entities.push_back(e);
            }
        }

//        removeDeadEntities(m_entities);

        m_entitiesToAdd.clear();
    }
    void removeDeadEntities(std::vector<shared_ptr<Entity1>>& entities) {
        auto itr = std::remove_if(entities.begin(), entities.end(), [](const std::shared_ptr<Entity1> e) {
            return !e->isActive();
        });
        entities.erase(itr, entities.end());
    }
    void removeEntity(std::shared_ptr<Entity1> targetEntity) {
        auto itr = std::remove_if(m_entities.begin(), m_entities.end(), [targetEntity](const std::shared_ptr<Entity1> e) {
            return e == targetEntity;
        });
        if(itr != m_entities.end()) {
            m_entities.erase(itr, m_entities.end());
        }
    }
};

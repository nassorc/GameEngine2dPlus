#pragma once

// don't add entites immediately. create a temp var then add to toadd vector

#include "Entity.h"
#include "Entity.cpp"
#include <vector>
#include <map>
#include <tuple>
#include <iostream>

typedef std::vector<std::shared_ptr<Entity>> EntityVector;


class EntityManager {
    EntityVector                          m_entities;
    std::map<std::string, EntityVector>   m_entityMap;      // map contains the entities grouped by tags
    EntityVector                          m_entitiesToAdd;            // wait line to render next frame
    size_t                                m_totalEntities = 0;
    
    void removeDeadEntities(EntityVector& vec);

public:
    EntityManager(); 
    void init(); 
    void update();
    std::shared_ptr<Entity> addEntity(std::string tag);
    EntityVector getEntities(); 
    EntityVector getEntities(std::string tag); 
    int test() {
      return 101;
    }
};

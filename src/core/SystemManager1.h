//
// Created by Matthew Crossan on 10/1/23.
//

#ifndef LIGHTCASTING_SYSTEMMANAGER1_H
#define LIGHTCASTING_SYSTEMMANAGER1_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include "../include/System.h"

using namespace std;

class SystemManager1 {
private:
    size_t numberOfSystems = 0;
    std::unordered_map<std::string, std::shared_ptr<System>> m_systems;
    std::unordered_map<std::string, Signature>     m_systemSignatures;

//
//    std::unordered_map<std::string, size_t> m_systemToIndexMap;
//    std::vector<std::shared_ptr<System>> m_systemVec;


    // use typeName to query index
    // use index to modify, get, or delete system
    // This allows us to order the systems by first in

public:
    SystemManager1() {}

    template <class T>
    void registerSystem() {
        string typeName = typeid(T).name();
        assert(m_systems.find(typeName) == m_systems.end() &&
               "Cannot register a system more than once");

        m_systems.insert({typeName, make_shared<T>()});
        numberOfSystems++;
    }

    template <class T>
    void setSignature(Signature signature) {
        string typeName = typeid(T).name();
        assert(m_systems.find(typeName) != m_systems.end() &&
               "Cannot set signature of unregistered system");

        m_systemSignatures.insert({typeName, (Signature) signature});
    }

    template <class T>
    std::shared_ptr<T> getSystem() {
        string typeName = typeid(T).name();
        assert(m_systems.find(typeName) != m_systems.end() &&
               "Cannot get signature of unregistered system");


        return static_pointer_cast<T>(m_systems.find(typeName)->second);
    }

    void entitySignatureChange(shared_ptr<Entity1> entity, Signature entitySignature) {
        for(auto& [typeName, system] : m_systems) {
            // use entity signature to check if it tests against system's signature
            // string typeName = typeid(T).name();
            Signature systemSignature = m_systemSignatures[typeName];     // get system's signature

            if ((systemSignature & entitySignature) == systemSignature) {
                // if entity atleast has the same bits set as the system's signature
                // insert does nothing if it item already exists
                system->m_entitiesToAdd.push_back(entity);
//                system->m_entitiesToAdd.insert(entity);
            } else {
                // if entity's signature changed
                // erase does nothing if it item doesn't exists
//                system->m_entities.erase(entity);
                system->removeEntity(entity);
            }
        }
    }

    void entityDestroyed(shared_ptr<Entity1> target) {
        std::cout << "destroying system entity" << std::endl;
        for(auto& [typeName, system] : m_systems) {
            system->removeEntity(target);
        }
    }

    template<class T>
    void removeEntityFromSystem(shared_ptr<Entity1> target) {
        std::string typeName = typeid(T).name();
        m_systems[typeName]->removeEntity(target);
    }
    void updateSystems() {
        for(auto& [typeName, system] : m_systems) {
            system->update();
        }
    }
};


#endif //LIGHTCASTING_SYSTEMMANAGER1_H

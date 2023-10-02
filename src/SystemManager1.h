//
// Created by Matthew Crossan on 10/1/23.
//

#ifndef LIGHTCASTING_SYSTEMMANAGER1_H
#define LIGHTCASTING_SYSTEMMANAGER1_H

#include <iostream>
#include <unordered_map>
#include <string>
#include "./include/System.h"

using namespace std;

class SystemManager1 {
private:
    unordered_map<string, shared_ptr<System>> m_systems;
    unordered_map<string, Signature> m_systemSignatures;

public:
    SystemManager1() {}

    template <class T>
    void registerSystem() {
        string typeName = typeid(T).name();
        assert(m_systems.find(typeName) == m_systems.end() &&
               "Cannot register a system more than once");

        m_systems.insert({typeName, make_shared<T>()});
    }

    template <class T>
    void setSignature(Signature signature) {
        string typeName = typeid(T).name();
        assert(m_systems.find(typeName) != m_systems.end() &&
               "Cannot set signature of unregistered system");

        m_systemSignatures.insert({typeName, (Signature) signature});
    }

    void entitySignatureChange(shared_ptr<Entity1> entity, Signature entitySignature) {
        for(auto& [typeName, system] : m_systems) {
            // use entity signature to check if it tests against system's signature
            // string typeName = typeid(T).name();
            Signature systemSignature = m_systemSignatures[typeName];     // get system's signature

            std::cout << entity->getSignature() << " " << entitySignature << " " << systemSignature << std::endl;

            if ((systemSignature & entitySignature) == systemSignature) {
                // if entity atleast has the same bits set as the system's signature
                // insert does nothing if it item already exists
                system->m_entities.insert(entity);
            } else {
                // if entity's signature changed
                // erase does nothing if it item doesn't exists
                system->m_entities.erase(entity);
            }
        }
    }

    void eraseEntity(shared_ptr<Entity1> entity) {
        for(auto& [typeName, system] : m_systems) {
            Signature systemSignature = m_systemSignatures[typeName];     // get system's signature
            Signature entitySignature = entity->getSignature();

            if ((systemSignature & entitySignature) == systemSignature) {
                system->m_entities.erase(entity);
            }
        }
    }
    void updateSystems() {
        for(auto& [typeName, system] : m_systems) {
            system->update();
        }
    }
};


#endif //LIGHTCASTING_SYSTEMMANAGER1_H

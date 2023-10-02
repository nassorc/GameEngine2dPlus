//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <string>
#include <bitset>
#include "Components1.h"
//#include "ComponentManager1.h"

using namespace std;
using Signature = bitset<5>;

class EntityManager1;
class ComponentManager1;

class Entity1 {
private:
    size_t m_id;
    string m_tag;
    bool   m_active;
    Signature m_signature;
    std::shared_ptr<ComponentManager1> m_componentManager;
//    EntityManager& m_entityManager;

public:
    Entity1(size_t id, string tag, std::shared_ptr<ComponentManager1> cm);

    size_t getId();
    string getTag();
    bool isActive() const {
        return m_active;
    }
    void destroy();
    void setSignature(Signature signature);
    Signature getSignature();

    // overloaded operators
    bool operator< (const Entity1& rhs) const;
};

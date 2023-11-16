//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <string>
#include <bitset>
#include "Types.h"

using namespace std;
//using Signature = bitset<5>;

class EntityManager1;

class Entity1 {
private:
    size_t m_id;
    string m_tag;
    bool   m_active;
    Signature m_signature;
//    EntityManager& m_entityManager;

public:
    Entity1(size_t id, string tag);

    size_t id();
    string tag();
    bool isActive() const {
        return m_active;
    }
    void setNotActive() {
        m_active = false;
    }
    void destroy();
    void setSignature(Signature signature);
    Signature getSignature();

    // overloaded operators
    bool operator< (const Entity1& rhs) const;
};

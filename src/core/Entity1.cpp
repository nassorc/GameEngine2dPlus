//
// Created by Matthew Crossan on 10/1/23.
//
#include "Entity1.h"

#include <string>
#include <bitset>

Entity1::Entity1(size_t id, string tag)
: m_id(id)
, m_tag(tag)
, m_active(true) {
}


size_t Entity1::id() { return m_id; }
string Entity1::tag() { return m_tag; }
//bool Entity1::isActive() { return m_active; }
void Entity1::destroy() { m_active = false; }
void Entity1::setSignature(Signature signature) {
    m_signature = signature;
}
Signature Entity1::getSignature() {
    return m_signature;
}

// overloaded operators
bool Entity1::operator< (const Entity1& rhs) const {
    return m_id < rhs.m_id;
}

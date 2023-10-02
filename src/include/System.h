//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <set>
#include "../Entity1.h"

class SystemManager1;

class System {
    friend class SystemManager1;

protected:
    std::set<shared_ptr<Entity1>> m_entities;

public:
    System() = default;
    virtual void update() {}
};

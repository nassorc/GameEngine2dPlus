//
// Created by Matthew Crossan on 10/1/23.
//

#ifndef LIGHTCASTING_SYSTEM1_H
#define LIGHTCASTING_SYSTEM1_H

#include <set>
#include "Entity1.h"
//#include "SystemManager1.h"
class SystemManager1;

class System1 {
    friend class SystemManager1;

protected:
    std::set<shared_ptr<Entity1>> m_entities;

public:
    System1() = default;
    virtual void update() {}
};

#endif //LIGHTCASTING_SYSTEM1_H

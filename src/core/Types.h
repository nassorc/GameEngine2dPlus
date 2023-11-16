//
// Created by Matthew Crossan on 10/3/23.
//

#pragma once
#include <bitset>
#include <map>
#include "../math/Vec2.h"

class CoreManager;
class Entity1;
class Physics1;

namespace sf {
    class RenderWindow;
}

// world.set<ComponentType>(entity);
// world.get<ComponentType>(entity);
// world.registerSystem<>()
// world.egisterComponetn<>()
// world.window()

struct Global {
    CoreManager* coreManager = nullptr;
    sf::RenderWindow* window = nullptr;

    Global() {}
//    Global(CoreManager* coreManager, sf::RenderWindow* window)
//        : coreManager(coreManager)
//        , window(window)
//        {}
};

struct Intersect {
    bool result;
    Vec2 pos;
};


typedef std::vector<std::shared_ptr<Entity1>> Entity1Vector;
typedef std::map<std::string, Entity1Vector> Entity1Map;
using Signature = std::bitset<32>;
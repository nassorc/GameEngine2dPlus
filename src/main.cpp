// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include "GameEngine.h"

#include "EntityManager1.h"
#include "ComponentManager1.h"
#include "Components1.h"
#include "System1.h"
#include "SystemManager1.h"
#include "CoreManager.h"
#include "System1.h"

class PhysicsSystem1 : public System1 {
public:
    PhysicsSystem1() {}
    void update() {
        for(auto e : m_entities) {
            std::cout << e->tag() << std::endl;
        }
    }
};

struct Transform {
    int x, y;
};

int main()
{
//    CoreManager cm;
//    cm.init();
//    // register components
//    cm.registerComponent<Position>();
//    cm.registerComponent<Transform>();
////    std::cout << "Position signature " << cm.getComponentType<Position>() << std::endl;
//
//    // register systems and signatures
//    cm.registerSystem<PhysicsSystem1>();
//    {
//        Signature signature;
//        signature.set(cm.getComponentType<Position>());
//        signature.set(cm.getComponentType<Transform>());
//
//        cm.setSystemSignature<PhysicsSystem1>(signature);
//    }
//    auto player = cm.createEntity("player");
//    auto npc = cm.createEntity("npc");
//    auto npc2 = cm.createEntity("npc");
//
//    cm.addComponent(player, Position{1,99});
//    cm.addComponent(player, Transform{1,99});
//    cm.addComponent(npc, Position{10,20});
//    cm.addComponent(npc2, Position{10,30});
//
//    std::cout << cm.getComponent<Position>(player).y << std::endl;
//    std::cout << cm.getComponent<Position>(npc).y << std::endl;
//    std::cout << cm.getComponent<Position>(npc2).y << std::endl;
//
//    cm.updateSystems();

    GameEngine game("../bin/config/config.json");
    game.run();
}


//
// Created by Matthew Crossan on 10/6/23.
//

#include "HitboxSystem.h"

#include "../components/Components.h"
#include "../core/World.h"
extern World world;

HitboxSystem::HitboxSystem() : System() {

}

void HitboxSystem::update() {
    System::update();
    for(auto& entity : m_entities) {
        auto& hitbox = world.get<CHitbox>(entity);
        auto& bbox = world.get<CBoundingBox>(entity);

        auto& source = world.get<CHitbox>(entity).source;

        if(hitbox.behavior == HitboxBehavior::Follow) {
            auto& sourcePos = world.get<CTransform>(source).pos;
            float direction = world.get<CTransform>(source).direction;

            auto& entityPos = world.get<CTransform>(entity);
            entityPos.pos = sourcePos + Vec2(bbox.offset.x * direction, bbox.offset.y);
        }
        if(!entity->isActive()) {
//            if(world.has<CInput>(source)) {
//                std::cout << source->tag();
//                auto& input = g_coreManagaer.getComponent<CInput>(source);
//                input.canAttack = true;
//                input.attack = false;
//            }

// TODO: IMPLEMENT REMOVE ENTITY IN WORLD ======================
//            world.removeEntity(entity);
            world.coreManager->removeEntity(entity);
// TODO: IMPLEMENT REMOVE ENTITY IN WORLD ======================
        }

//        string sourceTag = world.getComponent<CHitbox>(hitbox).source->tag();
//        std::cout << world.getComponent<CHitbox>(hitbox).source->tag() << std::endl;
    }
}
void HitboxSystem::hitboxFollowSource() {

}

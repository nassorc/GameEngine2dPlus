//
// Created by Matthew Crossan on 10/3/23.
//

#include "GravitySystem.h"

#include "../core/CoreManager.h"
#include "../components/Components.h"
#include "../core/Types.h"
#include "../scenes/SceneBattle.h"

#include "../core/World.h"
extern World world;
extern PlayerConfig g_playerConfig;

GravitySystem::GravitySystem() {}

void GravitySystem::update() {
    System::update();

    for (auto &e: m_entities) {
        auto& transform = world.get<CTransform>(e);
        auto& velocity = world.get<CTransform>(e).velocity;
        auto& gravity = world.get<CGravity>(e).gravity;
        auto& input = world.get<CInput>(e);

//        if(world.hasComponent<CInput>(e)) {
//            auto& input = world.getComponent<CInput>(e);
//
//            if(velocity.y > 0 && !input.canJump) {
//                g_playerConfig.gravityMultiplier = 1.3;
//            }
//            else if(velocity.y > -10 && velocity.y < 10 && !input.canJump) {
//                g_playerConfig.gravityMultiplier = 0.4;
//            }
//            velocity.y += gravity * g_playerConfig.gravityMultiplier;
//        }
//        else {
//            velocity.y += gravity;
//        }
//        velocity = Vec2{0, std::min(15.f, transform.velocity.y + gravity)};

        if(velocity.y > -10 && velocity.y < 10 && !input.canJump) {
            g_playerConfig.gravityMultiplier = 0.00001;
        }

        if(velocity.y > 10 && !input.canJump) {
            g_playerConfig.gravityMultiplier = 1.3;
        }
        else if(velocity.y > -10 && velocity.y < 10 && !input.canJump) {
            g_playerConfig.gravityMultiplier = 0.4;
        }

        velocity.y += gravity * g_playerConfig.gravityMultiplier;

        for (auto &e1: m_entities) {
            if(!world.has<CCollide1>(e1)) { continue; }
            for(auto& collision : world.get<CCollide1>(e1).collisions) {
                if(collision.type == Collision::CollisionType::TOP) {
//                    g_playerConfig.gravityMultiplier = 1;
//                    g_playerConfig.force = 1;
//                    if(world.hasComponent<CInput>(e)) {
//                        world.getComponent<CInput>(e).isGrounded = true;
//                    }
                }
            }

        }
    }
}

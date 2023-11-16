//
// Created by Matthew Crossan on 10/7/23.
//

#include "AISystem.h"

#include "../components/Components.h"
#include "../core/CoreManager.h"
#include "../scenes/SceneBattle.h"
#include "../math/Physics1.h"

#include "../core/World.h"
extern World world;

//extern CoreManager world;
//extern PlayerConfig g_playerConfig;

AISystem::AISystem() : System() {

}

void AISystem::update() {
    System::update();
    for (auto ai : m_entities) {
        auto& transform = world.get<CTransform>(ai);
//        transform.velocity = Vec2{0.f, 0.f};

        if(world.has<CAISeekBehavior>(ai)) {
            Vec2 delta;
            for(auto target : world.query<CPlayer>()) {
                auto& targetPos = world.get<CTransform>(target).pos;
                delta = targetPos - transform.pos;
            }
            float steeringScale = 0.04;
            Vec2 desired = Vec2::normalize(delta) * 10;                // desired direction velocity (target)
            Vec2 steering = desired - transform.velocity;                 // vector from current velocity to desired vector
            Vec2 actual = Vec2(steering.x, steering.y) * steeringScale;   // scale down steering, so it won't go from current
                                                                           // vector to desired vector
            transform.velocity = actual + transform.velocity;             // add actual steering to current velocity
        }
    }
}
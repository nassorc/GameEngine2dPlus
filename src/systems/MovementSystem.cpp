//
// Created by Matthew Crossan on 10/2/23.
//

#include "MovementSystem.h"
#include "../components/Components.h"
#include "../scenes/SceneBattle.h"
#include "../core/World.h"
extern World world;
extern PlayerConfig g_playerConfig;

MovementSystem::MovementSystem() : System() {

}

void MovementSystem::update() {
    System::update();
    for (auto &e : m_entities) {
        auto &transform = world.get<CTransform>(e);
        if(transform.velocity.x > 0) {
            transform.direction = Direction::RIGHT;
        }
        else if(transform.velocity.x < 0) {
            transform.direction = Direction::LEFT;
        }
        // set previous position. used to determine the direction of the aabb collision algorithm
        transform.prevPos = transform.pos;
        // clamp the speed in both axes an entity can travel in
        transform.pos += Vec2{std::min(16.f, transform.velocity.x), std::min(16.f, transform.velocity.y)};
    }
}

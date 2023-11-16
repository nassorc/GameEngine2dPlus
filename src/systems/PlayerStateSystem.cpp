//
// Created by Matthew Crossan on 10/6/23.
//

#include "PlayerStateSystem.h"

#include "../components/Components.h"
#include "../scenes/SceneBattle.h"
#include "../core/World.h"
extern World world;
extern PlayerConfig g_playerConfig;

PlayerStateSystem::PlayerStateSystem() : System() {}
void PlayerStateSystem::update() {
    System::update();
    for (auto e : m_entities) {
        auto& state = world.get<CState>(e);
        if (world.has<CCollide1>(e)) {
            for (auto& collision : world.get<CCollide1>(e).collisions) {
                if (collision.type == Collision1::CollisionType::TOP) {
                    g_playerConfig.gravityMultiplier = 1;
                    g_playerConfig.force = 1;
                    world.get<CInput>(e).isGrounded = true;
                }
                else if (collision.type == Collision1::CollisionType::LEFT || collision.type == Collision1::CollisionType::RIGHT) {
                    world.get<CInput>(e).canWallJump = true;
                }
                if (!(collision.type == Collision1::CollisionType::LEFT) && !(collision.type == Collision1::CollisionType::RIGHT)) {
                    world.get<CInput>(e).canWallJump = false;
                }
            }
        }
    }
}

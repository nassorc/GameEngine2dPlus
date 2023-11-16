//
// Created by Matthew Crossan on 10/1/23.
//

#include "PlayerInputSystem.h"
#include "../components/Components.h"
#include "../scenes/SceneBattle.h"
#include "../core/World.h"
extern World world;
extern PlayerConfig g_playerConfig;

PlayerInputSystem::PlayerInputSystem() : System() {

}

void PlayerInputSystem::update() {
    System::update();
    for(auto e : m_entities) {
        auto& transform = world.get<CTransform>(e);
        auto& input = world.get<CInput>(e);

        Vec2 velocity = {0, 0};

        if (input.up) {
            velocity.y = -1 * g_playerConfig.speed;
        }
        else if(input.down) {
            velocity.y = g_playerConfig.speed;
        }
        else if (input.left) {
            velocity.x = -1 * g_playerConfig.speed;
        }
        else if(input.right) {
            velocity.x = g_playerConfig.speed;
        }

        transform.velocity = Vec2{std::min(velocity.x, g_playerConfig.maxSpeed), std::min(velocity.y, g_playerConfig.maxSpeed)};

        // SET PREVIOUS POSITION
        transform.prevPos = transform.pos;
    }
}

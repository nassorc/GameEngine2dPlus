//
// Created by Matthew Crossan on 10/1/23.
//

#include "../include/PlayerInputSystem.h"
#include "../CoreManager.h"
#include "../Components.h"
#include "../Vec2.h"

extern CoreManager g_coreManager;

PlayerInputSystem::PlayerInputSystem() {

}

void PlayerInputSystem::update() {
    for(auto e : m_entities) {
        Vec2 velocity{0, 0};

        auto& transform = g_coreManager.getComponent<CTransform>(e);
        auto& input = g_coreManager.getComponent<CInput>(e);

        std::cout << e->tag() << std::endl;

        if(input.up) {
            std::cout << "W pressed" << std::endl;
        }

    }
}

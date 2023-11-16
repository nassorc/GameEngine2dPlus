//
// Created by Matthew Crossan on 10/3/23.
//


#include <SFML/Graphics.hpp>

#include "RenderSystem.h"
#include "../components/Components.h"
#include "../scenes/SceneBattle.h"
#include "../core/Types.h"
#include "../core/World.h"

extern World world;

RenderSystem::RenderSystem() {

}

void RenderSystem::update() {
    System::update();
    world.window->clear(sf::Color::Black);

    for(auto& e: m_entities) {
        if(world.has<CAnimation>(e)) {
            auto& animation = world.get<CAnimation>(e).animation;
            auto& newPos = world.get<CTransform>(e).pos;
//            rect.setPosition(newPos.x, newPos.y);
            animation.getSprite().setPosition(newPos.x, newPos.y);
            world.window->draw(animation.getSprite());
        }
        else if(world.has<CRect>(e)) {
            auto& rect = world.get<CRect>(e).rect;
            auto& newPos = world.get<CTransform>(e).pos;
            rect.setPosition(newPos.x, newPos.y);
            world.window->draw(rect);
        }
    }
}

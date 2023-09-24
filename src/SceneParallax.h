//
// Created by Matthew Crossan on 9/16/23.
//

#pragma once

#include "Scene.h"
#include "Entity.h"
#include "GameEngine.h"
#include "Physics.h"

class SceneParallax : public Scene {
private:
    bool            m_drawGrid = false;
    sf::ConvexShape m_shape;
    sf::Color       m_backgroundColor = sf::Color(230, 230, 240);
    Vec2            m_mousePos = Vec2(0,0);
    Physics         m_physics;
    sf::Sprite     m_background1;
    sf::Sprite     m_background2;
    sf::Sprite     m_background3;
    std::vector<sf::Sprite> layers;
    std::shared_ptr<Entity> m_player;
    sf::View       m_camera = sf::View(m_game->window().getDefaultView());

public:
    SceneParallax(GameEngine *gameEngine, Assets &assetManager, const std::string &config);
    void init(const std::string& config);
    void update() override;
    void sDoAction(const Action &action) override;
    void sMovement();
    void sRender() override;
    void onEnd() override;
};

//
// Created by Matthew Crossan on 9/14/23.
//
#pragma once

#include "../scenes/Scene.h"
#include "../core/GameEngine.h"
#include "Physics.h"

class ScenePlayground : public Scene{
private:
    bool            m_drawGrid = false;
    sf::ConvexShape m_shape;
    sf::Color       m_backgroundColor = sf::Color(230, 230, 240);
    Vec2            m_mousePos = Vec2(0,0);
    Physics         m_physics;


public:
    ScenePlayground(GameEngine *gameEngine, Assets &assetManager);
    void init();
    void update() override;
    void sDoAction(const Action &action) override;
    void sRender();
    void onEnd() override;


    void drawLine(const Vec2& p1, const Vec2& p2);
    void drawLine(const Vec2& p1, const Vec2& p2, sf::Color color);
    void drawGrid();
    void drawCursor();
    void drawCursorTextPos();
    void drawLight();
};
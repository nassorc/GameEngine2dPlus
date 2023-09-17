//
// Created by Matthew Crossan on 9/11/23.
//
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "Scene.h"
#include "GameEngine.h"
#include "Physics.h"

enum Camera {
    Follow,
    Room
};


class ScenePlay : public Scene {
    struct PlayerConfig {int X, Y, BX, BY, S, ML;};
    struct LevelConfig {int TILE_WIDTH{64}, TILE_HALF_WIDTH{32};};
private:
    sf::View                m_view;
    Camera                  m_camera = Camera::Room;
    std::shared_ptr<Entity> m_player = nullptr;
    PlayerConfig            m_playerConfig;
    LevelConfig             m_levelConfig;
    Physics                 m_physics;
    bool                    m_drawCollisions  = false;
    bool                    m_drawTextures    = true;
    bool                    m_paused          = false;

    void init(const std::string& levelPath);
    void loadLevel(const std::string& levelPath);
    Vec2 gridToWorld(const Vec2& coords);
    void spawnPlayer();
    void spawnSword();
    void sMovement();
    void sCollision();
    void sAnimation();
    void sLifespan();
    void sAttack();
    void sHealth();
    void sCamera();
    void sAI();
    void onEnd() override;
    // HELPER FUNCTIONS
    void resetPlayerInput();
    void drawLine(const Vec2& p1, const Vec2& p2);
    /*
     * Function takes in room coordinates and tile coordinates, and
     * returns the Vec2 game world position of the center of the entity
     */
    Vec2 getPosition(const Vec2& room, const Vec2& tile);
    /*
     * roomx = (positionX - halfWidth - (tileX * width)) / windowX
     */
    Vec2 getRoomCoordinates(const Vec2& room, const Vec2& tile);


public:
    ScenePlay(GameEngine* gameEngine, const std::string& levelPath);
    virtual ~ScenePlay();
    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;
};
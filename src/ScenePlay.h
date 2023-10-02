//
// Created by Matthew Crossan on 9/11/23.
//
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "Scene.h"
#include "GameEngine.h"
#include "Physics.h"
#include "Camera.h"
#include "TargetCamera.h"


//enum Camera {
//    Follow,
//    Room
//};

class LevelLoader;
class ImageLevelLoader;
class TilesetLevelLoader;
class WorldObjectsLevelLoader;


class ScenePlay : public Scene {
    struct PlayerConfig {int X, Y, BX, BY, S, ML; float MAXSPEED{12}, gravityMultiplier{1};};
    struct LevelConfig {int TILE_WIDTH{64}, TILE_HALF_WIDTH{32};};

    friend class LevelLoader;
    friend class ImageLevelLoader;
    friend class TilesetLevelLoader;
    friend class WorldObjectsLevelLoader;

private:
//    LevelLoader*            m_loader = nullptr;
//    LevelLoader*            m_loaderTile = nullptr;
    std::map<std::string, LevelLoader *> m_levelLoaderMap;

    sf::View                m_view;
    float                   m_targetViewDiff = 12.f;
    float                   m_initialTargetViewDiff = 12.f;
    Camera*                  m_camera = nullptr;
//    Camera                  m_camera = Camera::Room;
    std::shared_ptr<Entity> m_player = nullptr;
    PlayerConfig            m_playerConfig;
    LevelConfig             m_levelConfig;
    Physics                 m_physics;
    bool                    m_editorModeOn    = false;
    bool                    m_drawCollisions  = false;
    bool                    m_drawTextures    = true;
    bool                    m_paused          = false;
    bool                    m_drawGrid        = false;

    void init(const std::string& levelPath);
    void loadLevel(const std::string& levelPath);
    Vec2 gridToWorld(const Vec2& coords);
    void spawnPlayer();
    void spawnSword();
    void sMovement();
    void sCollision();
    void sResolveTileCollision();
    void sAnimation();
    void sLifespan();
    void sAttack();
    void sIFrames();
    void sHealth();
    void sCamera();
    void sAI();
    void sParallax();
    void sGravity();
    void sDialogue();
    void onEnd() override;
    // HELPER FUNCTIONS
    void resetPlayerInput();
    void drawLine(const Vec2& p1, const Vec2& p2);
    void drawBoundingBox(std::shared_ptr<Entity> e);
    void drawHealthBar(std::shared_ptr<Entity> e);
    void drawGrid();
    void drawTextEnvironementInformation(std::string text);
    void setEntityState(std::shared_ptr<Entity> e, StateType state);
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
    ScenePlay(GameEngine *gameEngine, Assets &assetManager, const std::string &levelPath, bool editorModeOn = false);
    virtual ~ScenePlay();
    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;
};


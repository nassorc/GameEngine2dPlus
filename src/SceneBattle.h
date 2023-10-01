//
// Created by Matthew Crossan on 9/26/23.
//

#pragma once

#include "Scene.h"
#include "Physics.h"
#include "GameEngine.h"

class SceneBattle : public Scene {
    struct PlayerConfig {
        int width{64}, height{64}, x{32}, y{32};
        float speed{5};
        sf::Color fc{255,255,255};
    };
    struct WorldConfig {
        int tileWidth{64}, tileHeight{64}, tileSize{64};
        sf::Color fc{209, 75, 109};
    };
private:
    std::shared_ptr<Entity> m_player = nullptr;
    Physics                 m_physics;
    PlayerConfig            m_playerConfig;
    WorldConfig             m_worldConfig;
public:
    SceneBattle(GameEngine *gameEngine, Assets &assetManager);

    //overide from abstract base class
    void update() override;
    void sDoAction(const Action &action) override;
    void sRender() override;
    void onEnd() override;

    //systems
    void sMovement();
    void sPlayerInput();
    void sCollision();
    void sTileCollision();
    void sCollisionCleanup();
    void sHitDetection();
    void sInteract();
    void sGravity();
    void sState();
    void sAnimation();

    void init();
    void spawnPlayer();
    void spawnTile(const Vec2& tile);
    void spawnInteractable(const Vec2& tile);
    Vec2 calculateWorldPos();
};


//
// Created by Matthew Crossan on 9/26/23.
//

#pragma once

#include "Scene.h"
#include "../(deprecated)/Physics.h"
#include "../core/GameEngine.h"
#include "../core/Entity1.h"

class LevelLoader1;
class ImageLevelLoader1;
class TilesetLevelLoader1;
class WorldObjectsLevelLoader1;

struct PlayerConfig {
    int width{40}, height{64}, x{32}, y{32};
    float speed{50}, maxSpeed{30}, jumpForce{36}, acceleration={2}, gravityMultiplier{1}, gravity{3}, force{1};
    sf::Color fc{255,255,255};
};
//struct PlayerConfig { int width{40}, height{64}, x{32}, y{32};
//    float speed{10}, maxSpeed{28}, jumpForce{30}, acceleration={2}, gravityMultiplier{1}, gravity{3}, force{1};
//    sf::Color fc{255,255,255};
//};

class SceneBattle : public Scene {
    struct WorldConfig {
        int tileWidth{64}, tileHeight{64}, tileSize{64};
        sf::Color fc{59, 121, 219};
    };

    friend class LevelLoader1;
    friend class ImageLevelLoader1;
    friend class TilesetLevelLoader1;
    friend class WorldObjectsLevelLoader1;

private:
    std::map<std::string, LevelLoader1 *> m_levelLoaderMap;

    std::shared_ptr<Entity> m_player = nullptr;
    std::shared_ptr<Entity1> m_player1 = nullptr;
    Physics                 m_physics;
    PlayerConfig            m_playerConfig;
    WorldConfig             m_worldConfig;
public:
    SceneBattle(GameEngine *gameEngine, Assets &assetManager);

    //overide from abstract base class
    void update() override;
    void sDoAction(const Action &action) override;
    void onEnd() override;

    //systems
    void sHitDetection();
    void sState();

    void init();
    void loadLevel(std::vector<std::string> level);
    void spawnPlayer();
    void spawnTile(const Vec2& tile);
    void spawnInteractable(const Vec2& tile);
    void spawnEnemy(const Vec2& tile);
    void spawnFlyingEnemy(const Vec2& tile);
    void spawnHitbox(std::shared_ptr<Entity1> source, const Vec2 &pos, const Vec2& size, const Vec2& offset, float damage, bool active);
    void spawnBullet(const Vec2& source, const Vec2& dist);
    Vec2 calculateWorldPos();
};


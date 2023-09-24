//
// Created by Matthew Crossan on 9/11/23.
//

#include "ScenePlay.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "Scene.h"
#include "GameEngine.h"
#include "Physics.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

ScenePlay::ScenePlay(GameEngine *gameEngine, Assets &assetManager, const std::string &levelPath, bool editorModeOn)
    : Scene(gameEngine, assetManager)
    , m_editorModeOn(editorModeOn)
{
    m_entityManager = EntityManager();
    init(levelPath);
}

void ScenePlay::init(const std::string &levelPath) {
    registerAction(sf::Keyboard::Key::W, "JUMP");
    registerAction(sf::Keyboard::Key::A, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");
    registerAction(sf::Keyboard::Key::Space, "ATTACK");
    registerAction(sf::Keyboard::Key::P, "TOGGLE_PAUSE");
    registerAction(sf::Keyboard::Key::C, "SWITCH_CAMERA_MODE");
    registerAction(sf::Keyboard::Key::V, "TOGGLE_DRAW_COLLISIONS");
    registerAction(sf::Keyboard::Key::T, "TOGGLE_DRAW_TEXTURES");
    registerAction(sf::Keyboard::Key::Y, "TOGGLE_DRAW_GRID");
    loadLevel(levelPath);
}

void ScenePlay::onEnd() {

}

void ScenePlay::resetPlayerInput() {
    m_player->getComponent<CInput>().up = false;
    m_player->getComponent<CInput>().down = false;
    m_player->getComponent<CInput>().left = false;
    m_player->getComponent<CInput>().right = false;
    m_player->getComponent<CInput>().attack = false;
}

void ScenePlay::sDoAction(const Action &action) {
    auto& state = m_player->getComponent<CState>();
    if (action.type() == "START") {
        if (action.name() == "JUMP") {
            m_player->getComponent<CInput>().up = true;
        } else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = true;
        } else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = true;
        } else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = true;
        } else if (action.name() == "ATTACK") {
            if(m_player->getComponent<CInput>().canAttack) {
                m_player->getComponent<CInput>().canAttack = false;
                m_player->getComponent<CInput>().attack = true;
//                spawnSword();
            }
        } else if (action.name() == "TOGGLE_PAUSE") {
            m_paused = !m_paused;
        } else if (action.name() == "SWITCH_CAMERA_MODE") {
//            m_camera = (m_camera == Camera::Room) ? Camera::Follow : Camera::Room;
        } else if (action.name() == "TOGGLE_DRAW_COLLISIONS") {
            m_drawCollisions = !m_drawCollisions;
        } else if (action.name() == "TOGGLE_DRAW_TEXTURES") {
            m_drawTextures = !m_drawTextures;
        } else if (action.name() == "TOGGLE_DRAW_GRID") {
            m_drawGrid = !m_drawGrid;
        }
    }
    if (action.type() == "END") {
        if (action.name() == "JUMP") {
            m_player->getComponent<CInput>().up = false;
//            // if JUMP key pressed ended and jump ended early
//            if(!m_player->getComponent<CInput>().canJump) {
//                m_playerConfig.gravityMultiplier = 3;
//            }
        }
        else if (action.name() == "DOWN") m_player->getComponent<CInput>().down = false;
        else if (action.name() == "LEFT") m_player->getComponent<CInput>().left = false;
        else if (action.name() == "RIGHT") m_player->getComponent<CInput>().right = false;
    }

}

void ScenePlay::sRender() {
    if (!m_paused) { m_game->clearWindow(sf::Color(252, 216, 168)); }
    else { m_game->clearWindow(sf::Color(240, 198, 156)); }

    // DRAW ENTITIES WITH ANIMATION COMPONENTS
    for (auto &e: m_entityManager.getEntities()) {
        // DRAW ENTITIES WITH ANIMATION COMPONENT
        if (e->hasComponent<CAnimation>()) {
            auto &eTransform = e->getComponent<CTransform>();
            e->getComponent<CAnimation>().animation.getSprite().setPosition(eTransform.pos.x, eTransform.pos.y);
            if (m_drawTextures) {
                auto &sprite = e->getComponent<CAnimation>().animation.getSprite();
                m_game->draw(sprite);
            }
        }
        // DRAW ENTITIES WITH SPRITE COMPONENT
        if(e->hasComponent<CSprite>()) {
            auto& sprite = e->getComponent<CSprite>().sprite;
            auto& parallax = e->getComponent<CParallax>().parallax;
            m_game->draw(sprite);
        }
        // DRAW HEALTH BAR
        if(e->hasComponent<CHealth>()) {
            drawHealthBar(e);
        }
        // DEBUG: DRAW BOUNDING BOX
        if (m_drawCollisions && e->hasComponent<CBoundingBox>()) {
            drawBoundingBox(e);
        }
        // DEBUG: DRAW PATROL POINTS
        if (m_drawCollisions && e->hasComponent<CFollowPlayer>()) {
            auto& p1 = e->getComponent<CTransform>().pos;
            auto& p2 = m_player->getComponent<CTransform>().pos;
            drawLine(p1, p2);
        }

    }
    // DEBUG: DRAW GRID
    if(m_drawGrid) {
        drawGrid();
    }
    // TEST =======================================================
//    for (auto &e: m_entityManager.getEntities()) {
//        if(e->tag() == "p") {
//            auto size = e->getComponent<CAnimation>().animation.getSprite().getGlobalBounds().getSize();
//            std::cout << e->getComponent<CAnimation>().animation.getSprite().getGlobalBounds().getSize().y << std::endl;
//            sf::RectangleShape b(sf::Vector2f (size.x, size.y));
//            b.setPosition(149.333f / 2, 149.333f / 2);
//            b.setOutlineColor(sf::Color::Red);
//            b.setOutlineThickness(2);
//            b.setFillColor(sf::Color::Transparent);
//            m_game->draw(b);
//        }
//    }
//    std::cout << tile1->getComponent<CAnimation>().animation.getSprite().getOrigin().x << std::endl;
//
//    auto tile2 = m_entityManager.addEntity("testtile");
//    tile2->addComponent<CTransform>(Vec2{0 + 32, 0 + 32}, Vec2{0, 0}, 0);
//    tile2->addComponent<CAnimation>(m_assetManager.getAnimation("1"));
//    m_game->draw(tile2->getComponent<CAnimation>().animation.getSprite());
//
//    sf::RectangleShape r1(sf::Vector2f(64.f, 64.f));
//    r1.setFillColor(sf::Color(10, 10, 10));
//    r1.setPosition(0, 0);
//    m_game->window().draw(r1);

    // ~TEST =======================================================



}

void ScenePlay::update() {
    m_entityManager.update();
    sAI();
    sMovement();
    sParallax();
    sAttack();
    sHealth();
    sLifespan();
    sGravity();
    sCollision();
    sCamera();
    sAnimation();
    sRender();
    m_currentFrame++;
}

Vec2 ScenePlay::gridToWorld(const Vec2 &coords) {
    Vec2 worldCoords(coords.x * 64 + 32, coords.y * 64 + 32);
    return worldCoords;
}

void ScenePlay::loadLevel(const std::string &levelPath) {
    std::ifstream fin2("../bin/config/home.json");
    if (!fin2) {
        std::cerr << "Cannot load level" << std::endl;
        exit(-1);
    }

    json level = json::parse(fin2);
    json layers = level["layers"];

    for (auto& layer : layers) {
        if(layer["type"] == "imagelayer") {
            std::string textureName = layer["name"];
            if(!m_assetManager.hasTexture(textureName)) { continue; }

            bool repeatx = layer["repeatx"];
            float parallaxX = 0;
            float parallaxY = 0;
            if(layer.contains("parallaxx")) {
                parallaxX = layer["parallaxx"];
            }
            if(layer.contains("parallaxy")) {
                parallaxY = layer["parallaxy"];
            }
            Vec2 windowSize = m_game->getWindowSize();

            // set texture x repeating
            sf::Texture tex = m_assetManager.getTexture(textureName);
            Vec2 textureSize{(float) tex.getSize().x, (float) tex.getSize().y};
            Vec2 textureToWindowRatio{windowSize.x / textureSize.x, windowSize.y / textureSize.y};
            tex.setRepeated(repeatx);

            sf::Sprite sprite(m_assetManager.getTexture(textureName), sf::IntRect(0, 0, 1280 * 10, 768));
            sprite.scale(textureToWindowRatio.x, textureToWindowRatio.y);

            auto image =  m_entityManager.addEntity("image");
            image->addComponent<CSprite>(sprite);
            image->addComponent<CParallax>(parallaxX,parallaxY);
        }
        else if(layer["type"] == "tilelayer") {
            std::string textureName = layer["name"];
            std::vector<int> tilemap = layer["data"];
            int height = layer["height"];
            int width = layer["width"];

            int row = 0;
            int col = 0;

            // loop through each tile
            for (size_t tile=0; tile<height*width; tile++) {
                if(textureName == "terrain") {
//                    if(tilemap[tile] == 0) { continue; }
                    if(tile % 30 == 0 && tile != 0) {
                        row++;
                        col = 0;
                    }
                    if(tilemap[tile] != 0) {
                        auto e = m_entityManager.addEntity("terrain");
                        e->addComponent<CTransform>(Vec2(col*64 + 32,row*64 + 32), Vec2(0,0), 0);
                        e->addComponent<CBoundingBox>(Vec2(64, 64), true, true);
                        auto& eAnimation = e->addComponent<CAnimation>(m_assetManager.getAnimation(std::to_string(tilemap[tile]-1)));
                    }
                    col++;
                }
            }
        }
        else if(layer["type"] == "objectgroup") {
            if(layer["name"] == "playerspawn") {
                json object = layer["objects"][0];
                int spawnX = ((int) object["x"])*2.667;
                int spawnY = ((int)object["y"])*2.667;

                m_playerConfig.S = 7;
                m_playerConfig.ML = 5;
                m_playerConfig.BX = 64;
                m_playerConfig.BY = 64;

                m_playerConfig.X = spawnX;
                m_playerConfig.Y = spawnY;
            }
            if(layer["name"] == "shop") {
                json object = layer["objects"][0];
                auto& animation = m_assetManager.getAnimation("Shop");
                auto animationSize = animation.getSprite().getLocalBounds().getSize();
                auto spriteScale = animation.getSprite().getScale();

                int x = (float) object["x"] * 2.667;
                int y = (float) object["y"] * 2.667 - ((animationSize.y * spriteScale.y) / 2);
                auto e = m_entityManager.addEntity("shop");
                e->addComponent<CTransform>(Vec2{(float) x, (float) y}, Vec2{0, 0}, 0);
                e->addComponent<CAnimation>(animation);
                e->addComponent<CBoundingBox>(Vec2{animationSize.x * spriteScale.x , animationSize.y * spriteScale.y}, false, false);
            }
            if(layer["name"] == "enemyspawn") {
                json objects = layer["objects"];
                for(auto object : objects) {
                    auto& animation = m_assetManager.getAnimation("SlimeRun");
                    auto animationSize = animation.getSprite().getLocalBounds().getSize();
                    auto spriteScale = animation.getSprite().getScale();

                    int x = (float) object["x"] * 2.667;
                    int y = (float) object["y"] * 2.667 - ((animationSize.y * spriteScale.y) / 2);
                    auto e = m_entityManager.addEntity("enemy");
                    e->addComponent<CTransform>(Vec2{(float) x, (float) y}, Vec2{2, 0}, 0);
                    e->addComponent<CAnimation>(animation);
                    e->addComponent<CBoundingBox>(Vec2{animationSize.x, animationSize.y}, false, false);
//                    e->addComponent<CGravity>(0.6);

                }
            }
            if(layer["name"] == "enemyboundary") {
                json objects = layer["objects"];
                for(auto object : objects) {
                    int x = (float) object["x"] * 2.667;
                    int y = (float) object["y"] * 2.667;
                    auto e = m_entityManager.addEntity("enemyboundary");
                    e->addComponent<CTransform>(Vec2{(float) x + 32, (float) y + 32}, Vec2{0, 0}, 0);
                    e->addComponent<CBoundingBox>(Vec2{(float) m_levelConfig.TILE_WIDTH, (float) m_levelConfig.TILE_WIDTH}, false, false);
                }
            }
        }
    }

//    std::ifstream fin(levelPath);
//    if (!fin) {
//        std::cerr << "Could open level path: " << levelPath << std::endl;
//        exit(-1);
//    }
//
//    std::string type, aniName;
//    int rx, ry, x, y, bx, by, maxLife, speed, damage;
//    bool blocksVision, blocksMovement;
//
//    while (fin >> type) {
//        // skip comment line
//        if (type == "#") getline(fin, type, '\n');
//        std::transform(type.begin(), type.end(), type.begin(), ::toupper);
//
//        if (type == "TILE") {
//            fin >> aniName >> rx >> ry >> x >> y >> blocksMovement >> blocksVision;
//            auto tile = m_entityManager.addEntity("TILE");
//            // convert screen position to world coordinates
//            auto &tileAnimation = tile->addComponent<CAnimation>(
//                    m_assetManager.getAnimation(aniName)).animation;
//            tile->addComponent<CTransform>(getPosition(Vec2(rx, ry), Vec2(x, y)), Vec2(0, 0), 0);
//            tile->addComponent<CBoundingBox>(Vec2(64, 64), blocksMovement, blocksVision);
//        }
//        else if (type == "PLAYER") {
////            fin >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.BX >> m_playerConfig.BY >> m_playerConfig.S
////                >> m_playerConfig.ML;
//            fin >> x >> y >> bx >> by >> speed >> maxLife;
//        }
//        else if (type == "NPC") {
//            int numberOfPatrolPositions;
//            std::string AIBehavior;
//            fin >> aniName >> rx >> ry >> x >> y >> blocksMovement >> blocksVision >> maxLife >> damage >> AIBehavior >> speed;
//
//            auto enemy = m_entityManager.addEntity("ENEMY");
//            Vec2 worldPos =  getPosition(Vec2(rx, ry), Vec2(x, y));
//            enemy->addComponent<CTransform>(worldPos, Vec2(0, 0), 0);
//            enemy->addComponent<CAnimation>(m_assetManager.getAnimation(aniName)).animation.getSprite();
//            enemy->addComponent<CBoundingBox>(Vec2(64, 64), blocksMovement, blocksVision);
//            enemy->addComponent<CDamage>(damage);
//            enemy->addComponent<CHealth>(maxLife, maxLife);
//
//            if (AIBehavior == "Patrol") {
//                std::vector<Vec2> positions;
//                int n;
//                float patrolX, patrolY;
//                fin >> n;
//                while (n > 0) {
//                    fin >> patrolX >> patrolY;
//                    positions.push_back(getPosition(Vec2(rx, ry), Vec2(patrolX, patrolY)));
//                    n--;
//                }
//                enemy->addComponent<CPatrol>(positions, speed);
//            }
//            else {
//                enemy->addComponent<CFollowPlayer>(worldPos, speed);
//            }
//        }
//    }
//    // spawn player on level
    spawnPlayer();
}

void ScenePlay::sMovement() {
    // PLAYER MOVEMENT
    // initialize y with current player's y velocity for gravity
    Vec2 velocity = {0.0f, m_player->getComponent<CTransform>().velocity.y};
    auto &input = m_player->getComponent<CInput>();
    auto &state = m_player->getComponent<CState>().state;

    if (input.up && input.canJump) {
        state = "JUMPING";
        input.canJump = false;
        velocity.y = 10 * -1;
    }
    if (input.down) {
//        state = "RUNNING_DOWN";
        velocity.y = m_playerConfig.S;
    }
    if (input.left) {
        state = "RUNNING_LEFT";
        velocity.x = m_playerConfig.S * -1;
    }
    if (input.right) {
        state = "RUNNING_RIGHT";
        if(!input.canJump) {
            velocity.x = m_playerConfig.S;
        }
        else {
            velocity.x = m_playerConfig.S;
        }
    }
    if (input.attack) {
        state = "ATTACKING";
    }
    if (!input.up && !input.canJump && velocity.y == 0) {  // player released jump key
        m_player->getComponent<CInput>().canJump = true;
    }
    if (!input.canJump && velocity.y >= -1 && velocity.y <= 1) {  // player released jump key
        m_playerConfig.gravityMultiplier = 0.7;
    }
    if (!input.up && state == "RUNNING_UP") {
        state = "STANDING_UP";
    }
    if (!input.down && state == "RUNNING_DOWN") {
        state = "STANDING_DOWN";
    }
    if (!input.left && state == "RUNNING_LEFT") {
        state = "STANDING";
    }
    if (!input.right && state == "RUNNING_RIGHT") {
        state = "STANDING";
    }

//    Vec2 &playerPos = m_player->getComponent<CTransform>().pos;
//    playerPos += velocity;
    m_player->getComponent<CTransform>().velocity = velocity;

    // SET PREVIOUS POSITION
    m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos;

    // UPDATE ENEMY VELOCITY
//    for (auto& enemy : m_entityManager.getEntities("enemy")) {
//        enemy->getComponent<CTransform>().velocity.x = ;
//    }

    // update position
    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CTransform>()) {
            auto &transform = e->getComponent<CTransform>();
            transform.pos += transform.velocity;
//            e->getComponent<CAnimation>().animation.getSprite().setPosition(epos.x, epos.y);
        }
    }

}

void ScenePlay::spawnPlayer() {
    auto e = m_entityManager.addEntity("Blue");
    e->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y), Vec2(0, 0), 0);
    auto& animation = e->addComponent<CAnimation>(m_assetManager.getAnimation("Blue"));
    Vec2 animationSize{animation.animation.getSprite().getGlobalBounds().getSize().x, animation.animation.getSprite().getGlobalBounds().getSize().y};

    e->addComponent<CBoundingBox>(Vec2{42.f, 86.f}, Vec2{0, 33}, false, false);
    e->addComponent<CHealth>(m_playerConfig.ML, m_playerConfig.ML);
    e->addComponent<CInvincibility>(30);
    e->addComponent<CInput>();
    e->addComponent<CState>();
    e->addComponent<CGravity>(0.6);
    m_player = e;
}

void ScenePlay::sCamera() {
    Vec2 windowSize = m_game->getWindowSize();
    m_camera = new PlayerCamera(m_player, sf::FloatRect(0.f, 0.f, (float) windowSize.x, (float) windowSize.y));
    m_camera->update(m_game->window());
    m_camera->render(m_game->window());
//    Vec2 windowSize = m_game->getWindowSize();
//    m_view = sf::View(sf::FloatRect(0.f, 0.f, (float) windowSize.x, (float) windowSize.y));
//    if (m_camera == Camera::Follow)
//    {
//        auto &playerPos = m_player->getComponent<CTransform>().pos;
//        if(playerPos.x >= m_view.getCenter().x)  {
//            m_view.move(sf::Vector2f(playerPos.x - ((float) windowSize.x / 2), 0));
//        }
//        m_game->setView(m_view);
//    }
//    else if (m_camera == Camera::Room)
//    {
//        auto &playerPos = m_player->getComponent<CTransform>().pos;
//        Vec2 roomPos = {(float) ((int) playerPos.x / (int) windowSize.x),
//                        (float) ((int) playerPos.y / (int) windowSize.y)};
//        if (playerPos.x < 0) roomPos.x -= 1;
//        if (playerPos.y < 0) roomPos.y -= 1;
//        m_view.setCenter((windowSize.x) * roomPos.x + (windowSize.x / 2),
//                         (windowSize.y) * roomPos.y + (windowSize.y / 2));
////        m_view.move(sf::Vector2f(playerPos.x - ((float)windowSize.x / 2), playerPos.y - ((float) windowSize.y / 2)));
//        m_game->setView(m_view);
//    }
}

Vec2 ScenePlay::getPosition(const Vec2 &room, const Vec2 &tile) {
    const int TILE_WIDTH = 64;
    const int TILE_HWIDTH = 32;
    Vec2 windowSize = m_game->getWindowSize();
    Vec2 pos(room.x * (float) windowSize.x + tile.x * TILE_WIDTH + TILE_HWIDTH,
             room.y * (float) windowSize.y + tile.y * TILE_WIDTH + TILE_HWIDTH);
    return pos;
}

void ScenePlay::sCollision() {
    // PLAYER TILE COLLISION
    for (auto &e: m_entityManager.getEntities("enemy")) {
        for (auto &b: m_entityManager.getEntities("enemyboundary")) {
            auto& eBox = e->getComponent<CBoundingBox>();
            auto& bBox = e->getComponent<CBoundingBox>();
            Vec2 collisionOverlap = m_physics.GetOverlap(e, b);
            if(collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                e->getComponent<CTransform>().velocity.x *= -1;
            }
        }

    }
    for (auto &e: m_entityManager.getEntities()) {
        if (!(e->hasComponent<CBoundingBox>())) { continue;}
        if (e->tag() == "PLAYER") { continue;}

        // BOUNDING BOX
        auto& bBox = e->getComponent<CBoundingBox>();

        // TILE PLAYER BOUNDING BOX COLLISION
        Vec2 collisionOverlap = m_physics.GetOverlap(m_player, e);
        if(e->tag() == "shop" && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
            std::cout << "buy some items?" << std::endl;
        }
        if (e->tag() == "terrain" && bBox.blockMovement && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
            Vec2 collisionPrevOverlap = m_physics.GetPreviousOverlap(m_player, e);
            auto &playerPos = m_player->getComponent<CTransform>().pos;
            auto &ePos = e->getComponent<CTransform>().pos;
            // LEFT SIDE COLLISION
            if (collisionPrevOverlap.y > 0 && playerPos.x > ePos.x) {
                m_player->getComponent<CTransform>().pos.x += collisionOverlap.x;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
                // RIGHT SIDE COLLISION
            else if (collisionPrevOverlap.y > 0 && playerPos.x < ePos.x) {
                m_player->getComponent<CTransform>().pos.x -= collisionOverlap.x;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
                // TOP SIDE COLLISION
            else if (collisionPrevOverlap.x > 0 && playerPos.y < ePos.y) {
//                if(!m_player->getComponent<CInput>().canJump) {
//                    m_player->getComponent<CState>().state = "STANDING";
//                }

//                m_player->getComponent<CState>().state = "STANDING";
                m_player->getComponent<CTransform>().pos.y -= collisionOverlap.y;
//                m_player->getComponent<CInput>().canJump = true;
                m_playerConfig.gravityMultiplier = 1;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
                // BOTTOM SIDE COLLISION
            else if (collisionPrevOverlap.x > 0 && playerPos.y > ePos.y) {
                m_player->getComponent<CTransform>().pos.y += collisionOverlap.y;
                m_player->getComponent<CTransform>().velocity.y = 0;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
        }

        // PLAYER ENEMY COLLISION (HEALTH AND INVINCIBILITY)
        if (e->tag() == "ENEMY" && e->hasComponent<CDamage>() && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
            if(!m_player->hasComponent<CInvincibility>()) {
                m_player->getComponent<CHealth>().remaining -= e->getComponent<CDamage>().damage;
                m_player->addComponent<CInvincibility>(30);
            }
        }
    }
    // SWORD ENEMY COLLISION (HEALTH)
    for (auto &sword: m_entityManager.getEntities("SWORD")) {
        if (!(sword->hasComponent<CBoundingBox>())) { continue;};
        for (auto &enemy: m_entityManager.getEntities("ENEMY")) {
            if (!(enemy->hasComponent<CBoundingBox>())) { continue;};

            auto& enemyBox = enemy->getComponent<CBoundingBox>();
            auto& swordBox = sword->getComponent<CBoundingBox>();
            // BOUNDING BOX
            Vec2 collisionOverlap = m_physics.GetOverlap(sword, enemy);
            if (collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                enemy->getComponent<CHealth>().remaining -= sword->getComponent<CDamage>().damage;
                sword->getComponent<CDamage>().damage = 0;
            }

        }
    }

    // ADVANCE IFRAMES
    for (auto &e: m_entityManager.getEntities()) {
        if(e->hasComponent<CInvincibility>()) {
            auto& iFrames = e->getComponent<CInvincibility>().iFrames;
            iFrames -= 1;
            if (iFrames < 1) {
                e->getComponent<CInvincibility>().has = false;
            }
        }
    }
}

void ScenePlay::sAnimation() {
    // UPDATE PLAYER ANIMATION
    auto &state = m_player->getComponent<CState>().state;
    auto &animation = m_player->getComponent<CAnimation>().animation;
    auto &input = m_player->getComponent<CInput>();

    if (state == "RUNNING_RIGHT" && animation.getName() != "BlueRun")
        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("BlueRun"));
    else if (state == "RUNNING_LEFT" && animation.getName() != "BlueRun") {
        auto& animation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("BlueRun"));
        animation.animation.getSprite().setScale(-1 * animation.animation.getSprite().getScale().x, animation.animation.getSprite().getScale().y);
    }
    else if (state == "JUMPING" && animation.getName() != "BlueJump1") {
        auto& animation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("BlueJump1"));
    }
    else if (state == "STANDING" && animation.getName() == "BlueRun") {
        sf::Vector2f scale = animation.getSprite().getScale();
        auto& animation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("Blue"));
        animation.animation.getSprite().setScale(scale.x, scale.y);
    }
    else if (state == "ATTACKING" && animation.getName() != "BlueAttack") {
        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("BlueAttack"));
    }

//
//    if (state == "STANDING_DOWN")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("Stand"));
//    else if (state == "STANDING_UP")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("StandUp"));
//    else if (state == "STANDING_LEFT")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("StandLeft"));
//    else if (state == "STANDING_RIGHT")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("StandRight"));
//    else if (state == "RUNNING_UP" && animation.getName() != "RunUp")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("RunUp"));
//    else if (state == "RUNNING_DOWN" && animation.getName() != "RunDown")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("RunDown"));
//    else if (state == "RUNNING_LEFT" && animation.getName() != "RunLeft")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("RunLeft"));
//    else if (state == "RUNNING_RIGHT" && animation.getName() != "RunRight")
//        m_player->addComponent<CAnimation>(m_assetManager.getAnimation("RunRight"));

    // UPDATE ANIMATION TICK
    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CAnimation>()) {
            auto& animation = e->getComponent<CAnimation>().animation;
            // ADD TRANSPARENCY TO ENTITIES WITH IFRAMES
            if(e->hasComponent<CInvincibility>()) {
                animation.getSprite().setColor(sf::Color(255,255,255, 100));
            }
            // SIMULATE ANIMATION
            animation.update();
        }
    }

}

void ScenePlay::spawnSword() {
    Vec2 &playerPos = m_player->getComponent<CTransform>().pos;
    auto sword = m_entityManager.addEntity("SWORD");
//    sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos, Vec2(0, 0), 0);
    sword->addComponent<CBoundingBox>(Vec2(64, 64), false, false);
    sword->addComponent<CDamage>();
    sword->addComponent<CLifespan>(20);

    auto& pState = m_player->getComponent<CState>().state;
    auto& pSize = m_player->getComponent<CAnimation>().animation.getSize();
    if(pState == "STANDING_RIGHT" || pState == "RUNNING_RIGHT") {
        sword->addComponent<CAnimation>(m_assetManager.getAnimation("SwordRight"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(64, 0), Vec2(0, 0), 0);
    }
    else if(pState == "STANDING_LEFT" || pState == "RUNNING_LEFT") {
        sword->addComponent<CAnimation>(m_assetManager.getAnimation("SwordLeft"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(-64, 0), Vec2(0, 0), 0);
    }
    else if(pState == "STANDING_UP" || pState == "RUNNING_UP") {
        sword->addComponent<CAnimation>(m_assetManager.getAnimation("SwordUp"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(0, -64), Vec2(0, 0), 0);
    }
    else if(pState == "STANDING_DOWN" || pState == "RUNNING_DOWN") {
        sword->addComponent<CAnimation>(m_assetManager.getAnimation("Sword"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(0, 64), Vec2(0, 0), 0);
    }
}

void ScenePlay::drawLine(const Vec2 &p1, const Vec2 &p2) {
    sf::Vertex line[] = {sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y)};
    m_game->window().draw(line, 2, sf::Lines);
}

void ScenePlay::sLifespan() {
    for(auto& e : m_entityManager.getEntities()) {
        if(!e->hasComponent<CLifespan>()) { continue; }
        auto& lifespan = e->getComponent<CLifespan>();
        if(lifespan.remaining < 0) {
            if(e->tag() == "SWORD") {
                m_player->getComponent<CInput>().canAttack = true;
            }
            e->destroy();
        }
        lifespan.remaining -= 1;
    }
}

void ScenePlay::sAttack() {
    for (auto& e : m_entityManager.getEntities()) {
        // UPDATE SWORD POSITION
        if(e->tag() == "SWORD") {
            auto& pState = m_player->getComponent<CState>().state;
            auto& pPos = m_player->getComponent<CTransform>().pos;
            if(pState == "STANDING_RIGHT" || pState == "RUNNING_RIGHT") {
                e->getComponent<CTransform>().pos = pPos + Vec2(64, 0);
            }
            if(pState == "STANDING_LEFT" || pState == "RUNNING_LEFT") {
                e->getComponent<CTransform>().pos = pPos + Vec2(-64, 0);
            }
            if(pState == "STANDING_UP" || pState == "RUNNING_UP") {
                e->getComponent<CTransform>().pos = pPos + Vec2(0, -64);
            }
            if(pState == "STANDING_DOWN" || pState == "RUNNING_DOWN") {
                e->getComponent<CTransform>().pos = pPos + Vec2(0, 64);
            }
        }
    }

}

void ScenePlay::sHealth() {
    for(auto& e : m_entityManager.getEntities()) {
        if(!e->hasComponent<CHealth>()) { continue;}
        auto& health = e->getComponent<CHealth>().remaining;
        if(health < 1) {
            e->destroy();
            if(e->tag() == "PLAYER") {
                spawnPlayer();
            }
        }
    }
}

void ScenePlay::sAI() {
    bool collision = false;
    for (auto& enemy : m_entityManager.getEntities("ENEMY")) {
        if(enemy->hasComponent<CFollowPlayer>()) {
            auto& playerPos = m_player->getComponent<CTransform>().pos;
            auto& enemyPos = enemy->getComponent<CTransform>().pos;
            for (auto& blockable : m_entityManager.getEntities()) {
                if (enemy->id() == blockable->id()) { continue;}
                if (blockable->tag() == "PLAYER") { continue;}
                if(m_physics.EntityIntersect(playerPos, enemyPos, blockable)) {
                    collision = true;
                }
            }
            if (!collision) {
                float speed = enemy->getComponent<CFollowPlayer>().speed;
                Vec2 delta = playerPos - enemyPos;
                Vec2 normalized = delta.normalize(delta.magnitude(delta));
                Vec2 velocity = normalized.scale(speed);
                enemy->getComponent<CTransform>().velocity = velocity;
            }
            else {
                float speed = enemy->getComponent<CFollowPlayer>().speed;
                Vec2 delta = enemy->getComponent<CFollowPlayer>().home - enemyPos;
                Vec2 normalized = delta.normalize(delta.magnitude(delta));
                Vec2 velocity = normalized.scale(speed);
                if(delta.magnitude(delta) <= 5) {
                    enemy->getComponent<CTransform>().velocity = Vec2{0.f, 0.f};
                    enemy->getComponent<CTransform>().pos = enemy->getComponent<CFollowPlayer>().home;
                }
                else {
                    enemy->getComponent<CTransform>().velocity = velocity;
                }
            }
        }
        if(enemy->hasComponent<CPatrol>()) {
            float speed = enemy->getComponent<CPatrol>().speed;
            auto& patrol = enemy->getComponent<CPatrol>();
            size_t idx = patrol.currentPosition;
            size_t distIdx = (idx + 1) % patrol.positions.size();
            auto& distinationPos = patrol.positions.at(distIdx);
            auto& currentPos = enemy->getComponent<CTransform>().pos;
            auto& initialPos = patrol.positions.at(patrol.currentPosition);

            Vec2 delta = distinationPos - currentPos;
            Vec2 normalized = delta.normalize(delta.magnitude(delta));
            Vec2 velocity = normalized.scale(speed);
            enemy->getComponent<CTransform>().velocity = velocity;

            if(delta.magnitude(delta) <= 5) {
                currentPos = distinationPos;
                patrol.currentPosition = (patrol.currentPosition + 1) % patrol.positions.size();
            }

        }
    }

}

ScenePlay::~ScenePlay() {

}

void ScenePlay::drawGrid() {
    Vec2 worldCenterCoord(m_view.getCenter().x, m_view.getCenter().y);
    Vec2 viewSize(m_view.getSize().x, m_view.getSize().y);
    Vec2 startingEdgeCoord(worldCenterCoord.x - (viewSize.x / 2), worldCenterCoord.y - (viewSize.y / 2));
    Vec2 endingEdgeCoord(worldCenterCoord.x + (viewSize.x / 2), worldCenterCoord.y + (viewSize.y / 2));
//        int leftCoord = worldCenterCoord.x - (viewSize.x / 2);
//        int rightCoord = worldCenterCoord.y - (viewSize.y / 2);

    // DRAW GRID
    for(int row=(startingEdgeCoord.y/64)-1; row<(endingEdgeCoord.y/64); row++) {
        for(int col=(startingEdgeCoord.x/64)-1; col<(endingEdgeCoord.x/64); col++) {
            // DRAW CELL
            sf::RectangleShape cell(sf::Vector2f(64, 64));
            float x = col * 64;
            float y = row * 64;
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::White);
            cell.setFillColor(sf::Color::Transparent);
            cell.setPosition(x, y);
            // DRAW CELL'S X AND Y LOCATION
            std::string l = "(" + std::to_string((int) (x / 64) ) + "," + std::to_string((int) (y / 64)) + ")";
            sf::Text locationString(l, m_assetManager.getDefaultFont(), 6);
            locationString.setPosition(x + 32, y + 32);
            locationString.setOrigin(locationString.getGlobalBounds().width/2, locationString.getGlobalBounds().height/2);
            locationString.setFillColor(sf::Color::Black);
            m_game->draw(cell);
            m_game->draw(locationString);
        }
    }
}

void ScenePlay::sParallax() {
    for (auto &e: m_entityManager.getEntities()) {
        if(e->hasComponent<CSprite>()) {
            auto& sprite = e->getComponent<CSprite>().sprite;
            auto& parallax = e->getComponent<CParallax>().parallax;
            Vec2 playerVelocity = m_player->getComponent<CTransform>().velocity;
            auto& playerState = m_player->getComponent<CState>().state;
            if(playerVelocity.x != 0) {
                sprite.move(-1 * (playerVelocity.x * parallax.x), 0);
            }
        }
    }
}

void ScenePlay::drawBoundingBox(std::shared_ptr<Entity> e) {
    auto &pos = e->getComponent<CTransform>().pos;
    auto &boundingBox = e->getComponent<CBoundingBox>();
    sf::RectangleShape box(sf::Vector2f(boundingBox.size.x, boundingBox.size.y));
    box.setPosition(pos.x + boundingBox.offset.x, pos.y + boundingBox.offset.y);
    box.setOrigin(boundingBox.halfSize.x, boundingBox.halfSize.y);
    box.setFillColor(sf::Color(255, 255, 255, 0));
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color(255, 255, 255));
    m_game->draw(box);

    // DRAW ENEMY POINTS
    sf::CircleShape point(6);
    point.setOrigin(3, 3);
    point.setPosition(pos.x, pos.y);
    point.setFillColor(sf::Color(0, 0, 0));
    m_game->draw(point);

    // DRAW AI PATROL POINTS
    if(e->hasComponent<CPatrol>()) {
        for(auto& p : e->getComponent<CPatrol>().positions) {
            sf::CircleShape point(6);
            point.setOrigin(3, 3);
            point.setPosition(p.x, p.y);
            point.setFillColor(sf::Color(0, 0, 0));
            m_game->draw(point);

        }
    }
}

void ScenePlay::drawHealthBar(std::shared_ptr<Entity> e) {
    auto& ePos = e->getComponent<CTransform>().pos;
    auto& health = e->getComponent<CHealth>();
    float healthRatio = (float) health.remaining / (float) health.total;

    sf::RectangleShape healthBackground(sf::Vector2f{ 64, 8});
    healthBackground.setOrigin(32, 32);
    healthBackground.setPosition(ePos.x, ePos.y- 20);
    healthBackground.setFillColor(sf::Color::Black);
    m_game->draw(healthBackground);

    sf::RectangleShape healthBar(sf::Vector2f{healthRatio * 64, 6});
    healthBar.setOrigin(32, 32);
    healthBar.setPosition(ePos.x, ePos.y- 20);
    healthBar.setFillColor(sf::Color::Red);
    m_game->draw(healthBar);

}

void ScenePlay::sGravity() {
    for (auto &e: m_entityManager.getEntities()) {
        if(e->hasComponent<CGravity>()) {
            float gravity = e->getComponent<CGravity>().gravity;
            float newGravity = e->getComponent<CTransform>().velocity.y + (gravity * m_playerConfig.gravityMultiplier);
            e->getComponent<CTransform>().velocity.y = std::min(newGravity, m_playerConfig.MAXSPEED);
        }
        e->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y;
    }
}



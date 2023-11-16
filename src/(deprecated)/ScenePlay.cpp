//
// Created by Matthew Crossan on 9/11/23.
//

#include "ScenePlay.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "../scenes/Scene.h"
#include "../core/GameEngine.h"
#include "Physics.h"
#include "../utils/levelParser/LevelLoader.h"
#include "../utils/levelParser/ImageLevelLoader.h"
#include "../utils/levelParser/TilesetLevelLoader.h"
#include "../utils/levelParser/WorldObjectsLevelLoader.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

ScenePlay::ScenePlay(GameEngine *gameEngine, Assets &assetManager, const std::string &levelPath, bool editorModeOn)
    : Scene(gameEngine, assetManager)
    , m_editorModeOn(editorModeOn)
{
    m_entityManager = EntityManager();
    init(levelPath);
}

void ScenePlay::init(const std::string &levelPath) {
    // register key actions
    registerAction(sf::Keyboard::Key::W, "JUMP");
    registerAction(sf::Keyboard::Key::A, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");
    registerAction(sf::Keyboard::Key::Space, "ATTACK");
    registerAction(sf::Keyboard::Key::P, "TOGGLE_PAUSE");
    registerAction(sf::Keyboard::Key::C, "SWITCH_CAMERA_MODE");
    registerAction(sf::Keyboard::Key::V, "TOGGLE_DRAW_COLLISIONS");
    registerAction(sf::Keyboard::Key::T, "TOGGLE_DRAW_TEXTURES");
    registerAction(sf::Keyboard::Key::Y, "TOGGLE_DRAW_GRID");

    // register strategies for loading level
    m_levelLoaderMap["imagelayer"] = new ImageLevelLoader();
    m_levelLoaderMap["tilelayer"] = new TilesetLevelLoader();
    m_levelLoaderMap["objectgroup"] = new WorldObjectsLevelLoader();
    loadLevel(levelPath);
}

void ScenePlay::loadLevel(const std::string &levelPath) {
    std::ifstream fin("../bin/config/home.json");
    if (!fin) {
        std::cerr << "Cannot load level" << std::endl;
        exit(-1);
    }

    json level = json::parse(fin);
    json layers = level["layers"];

    for (auto& layer : layers) {
        m_levelLoaderMap[layer["type"]]->loader(this, layer);
    }

    // spawn player on level
    spawnPlayer();
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
        else if (action.name() == "ATTACK") m_player->getComponent<CInput>().attack = false;
    }

}

void ScenePlay::sRender() {
    if (!m_paused) { m_game->clearWindow(sf::Color(252, 216, 168)); }
    else { m_game->clearWindow(sf::Color(240, 198, 156)); }

    m_game->window().setView(m_view);

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
    sIFrames();
    sAnimation();
    sCamera();
    sRender();
    sDialogue();
    m_currentFrame++;
}

Vec2 ScenePlay::gridToWorld(const Vec2 &coords) {
    Vec2 worldCoords(coords.x * 64 + 32, coords.y * 64 + 32);
    return worldCoords;
}

void ScenePlay::sMovement() {
    // PLAYER MOVEMENT
    // initialize y with current player's y velocity for gravity
    Vec2 velocity = {m_player->getComponent<CTransform>().velocity.x, m_player->getComponent<CTransform>().velocity.y};
    auto& transform = m_player->getComponent<CTransform>();
    auto &input = m_player->getComponent<CInput>();
    auto &state = m_player->getComponent<CState>().state;

    state = StateType::IDLE;

    if (input.up && input.canJump) {
        setEntityState(m_player, StateType::JUMPING);
        input.canJump = false;
        velocity.y = 10 * -1;
    }
    // player releases jump key before reaching apex
//    if (!input.canJump && velocity.y >= 0) {
//        m_playerConfig.gravityMultiplier *= 1.5;
//    }
    if (input.down) {
        velocity.y = m_playerConfig.S;
    }
    if(!input.left && !input.right) {
        velocity.x = 0;
    }
    if (input.left) {
        setEntityState(m_player, StateType::RUNNING);
        transform.direction = Direction::LEFT;
        velocity.x += 0.8 *  -1;
        if(velocity.x < -8.4) {
            velocity.x = -8.4;
        }
//        if(!input.canJump) {
//            velocity.x += 0.4 *  -1;
//
//            if(velocity.x > -8.4) {
//                velocity.x = -8.4;
//            }
//            else {
//            }
//        }
//        else {
//            velocity.x += std::max(1 * 1.4 * -1, -8.4);
//            std::cout << velocity.x << std::endl;
//        }
    }
    if (input.right) {
        setEntityState(m_player, StateType::RUNNING);
        transform.direction = Direction::RIGHT;
        velocity.x = m_playerConfig.S;
    }
    if (input.attack) {
        setEntityState(m_player, StateType::ATTACKING);
    }
    if (!input.up && !input.canJump && velocity.y == 0) {  // player released jump key
        m_player->getComponent<CInput>().canJump = true;
    }
    if (!input.canJump && velocity.y >= -1 && velocity.y <= 1) {  // player released jump key
        m_playerConfig.gravityMultiplier = 0.7;
    }

//    Vec2 &playerPos = m_player->getComponent<CTransform>().pos;
//    playerPos += velocity;
    m_player->getComponent<CTransform>().velocity = velocity;

    // SET PREVIOUS POSITION
    m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos;

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
    auto e = m_entityManager.addEntity("player");
    e->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y), Vec2(0, 0), 0);
    auto& animation = e->addComponent<CAnimation>(m_assetManager.getAnimation("Blue"));
    Vec2 animationSize{animation.animation.getSprite().getGlobalBounds().getSize().x, animation.animation.getSprite().getGlobalBounds().getSize().y};

    e->addComponent<CBoundingBox>(Vec2{42.f, 86.f}, Vec2{0, 33}, false, false);
    e->addComponent<CHealth>(m_playerConfig.ML, m_playerConfig.ML);
    e->addComponent<CInput>();
    e->addComponent<CState>();
    e->addComponent<CGravity>(0.6);
    e->addComponent<CMovable>();
//    e->addComponent<CHitbox>(Vec2{64.f, 86.f}, Vec2{64, 33}, false);
    m_player = e;
}

void ScenePlay::sCamera() {
//    Vec2 windowSize = m_game->getWindowSize();
//    m_camera = new TargetCamera(m_player, sf::FloatRect(0.f, 0.f, (float) windowSize.x, (float) windowSize.y));
//    m_camera->update(m_game->window());
//    m_camera->render(m_game->window());

    m_view = sf::View(sf::FloatRect(0.f, 0.f, (float) m_game->window().getSize().x, (float) m_game->window().getSize().y));

    Vec2 &targetPos = m_player->getComponent<CTransform>().pos;
    Vec2 &targetVelocity = m_player->getComponent<CTransform>().velocity;
    Vec2 &targetBBox = m_player->getComponent<CBoundingBox>().size;
    auto windowSize = m_game->window().getSize();

    if(targetPos.x - targetBBox.x >= m_view.getCenter().x)  {
        m_view.move(sf::Vector2f(targetPos.x - targetBBox.x - ((float) windowSize.x / 2), 0));
        m_targetViewDiff = m_initialTargetViewDiff;
    }
// SOMEWHAT WORKING ============================================================================
//    if(targetVelocity.x != 0 && targetPos.x - targetBBox.x >= m_view.getCenter().x)  {
//        m_view.move(sf::Vector2f(targetPos.x - targetBBox.x - ((float) windowSize.x / 2), 0));
//        m_targetViewDiff = m_initialTargetViewDiff;
////        if(targetVelocity.x != 0) {
////            m_view.move(-targetBBox.x, 0);
////        }
//    }
//    if(targetVelocity.x == 0 && targetPos.x >= m_view.getCenter().x && targetPos.x != m_view.getCenter().x) {
//        m_view.move(sf::Vector2f(targetPos.x - ((float) windowSize.x / 2), 0));
//        std::cout << m_targetViewDiff << std::endl;
//        if(m_targetViewDiff > 0) {
//            m_view.move(sf::Vector2f(m_initialTargetViewDiff - m_targetViewDiff, 0));
//            m_targetViewDiff -= 1;
//        }
//    }
// SOMEWHAT WORKING ============================================================================

//    if(targetPos.x - targetBBox.x >= m_view.getCenter().x)  {
//        m_view.move(sf::Vector2f(targetPos.x - targetBBox.x- ((float) windowSize.x / 2), 0));
//
//        if(m_player->getComponent<CTransform>().velocity.x == 0 && targetPos.x >= m_view.getCenter().x)  { int diff = std::abs(targetPos.x - m_view.getCenter().x);
//            std::cout << diff << std::endl;
//            if (m_targetViewDiff >= -0.1 && m_targetViewDiff <= 0.1 ) {// if zero
//                m_targetViewDiff = m_initialTargetViewDiff;
//            }
//            else {
//                // pan camera to catch up to player
//                m_view.move((m_initialTargetViewDiff - m_targetViewDiff), 0);
//                m_targetViewDiff -= 4;
//            }
//        }
//        else if (m_player->getComponent<CTransform>().velocity.x != 0) {
//            m_view.move(-targetBBox.x, 0);
//        }
//    }





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
    // loop through each entity
    // check for collision (if e1 has a boundingbox, and e2 blocks movement)
    // if collision add collision component with collider information
    for (auto &e1: m_entityManager.getEntities()) {
        if(!e1->hasComponent<CBoundingBox>()) { continue; }
        for (auto &e2: m_entityManager.getEntities()) {
            if(e1->tag() == e2->tag()) { continue; }
            if(!e1->hasComponent<CBoundingBox>()) { continue; }

            // HITBOX / ENTITY COLLISION


            auto& bBoxE1 = e1->getComponent<CBoundingBox>();
            auto& bBoxE2 = e2->getComponent<CBoundingBox>();
            Vec2 collisionOverlap = m_physics.GetOverlap(e1, e2);

            // TILE / PLAYER AND NPC COLLISION
            // if e1 is a moveable entity and e2 blocks movement and there is a collision between the two
            if(e1->hasComponent<CMovable>() && bBoxE2.blockMovement && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                Vec2 collisionPrevOverlap = m_physics.GetPreviousOverlap(e1, e2);
                e1->addComponent<CFrameCollision>(e2, collisionOverlap, collisionPrevOverlap);
            }
            if(e1->tag() == "enemy" && e2->tag() == "enemyboundary") {
                auto& eBox = e1->getComponent<CBoundingBox>();
                auto& bBox = e2->getComponent<CBoundingBox>();
                if(collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                    e1->getComponent<CTransform>().velocity.x *= -1;
                }
            }
            if(e1->tag() == "player" && e2->tag() == "shop" && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                e2->getComponent<CDialogue>().active = true;
            }
            // e1->has(CHurtBox) && e2->has(CHitBox)
            if (e1->tag() == "player" && e2->tag() == "enemy" && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                if(!e1->hasComponent<CInvincibility>()) {
                    e1->getComponent<CHealth>().remaining -= e2->getComponent<CDamage>().damage;
                    e1->addComponent<CInvincibility>(30);
                }
            }
            sResolveTileCollision();
        }
    }

    // PLAYER TILE COLLISION
//    for (auto &e: m_entityManager.getEntities("enemy")) {
//        for (auto &b: m_entityManager.getEntities("enemyboundary")) {
//            auto& eBox = e->getComponent<CBoundingBox>();
//            auto& bBox = e->getComponent<CBoundingBox>();
//            Vec2 collisionOverlap = m_physics.GetOverlap(e, b);
//            if(collisionOverlap.x > 0 && collisionOverlap.y > 0) {
//                e->getComponent<CTransform>().velocity.x *= -1;
//            }
//        }
//
//    }
    for (auto &e: m_entityManager.getEntities()) {
        if (!(e->hasComponent<CBoundingBox>())) { continue;}
        if (e->tag() == "PLAYER") { continue;}

        // BOUNDING BOX
        auto& bBox = e->getComponent<CBoundingBox>();

        // TILE PLAYER BOUNDING BOX COLLISION
        Vec2 collisionOverlap = m_physics.GetOverlap(m_player, e);
//        if(e->tag() == "shop" && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
//            e->getComponent<CDialogue>().active = true;
//        }
//        if (e->tag() == "terrain" && bBox.blockMovement && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
//            Vec2 collisionPrevOverlap = m_physics.GetPreviousOverlap(m_player, e);
//            auto &playerPos = m_player->getComponent<CTransform>().pos;
//            auto &ePos = e->getComponent<CTransform>().pos;
//            // LEFT SIDE COLLISION
//            if (collisionPrevOverlap.y > 0 && playerPos.x > ePos.x) {
//                m_player->getComponent<CTransform>().pos.x += collisionOverlap.x;
////                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
//            }
//                // RIGHT SIDE COLLISION
//            else if (collisionPrevOverlap.y > 0 && playerPos.x < ePos.x) {
//                m_player->getComponent<CTransform>().pos.x -= collisionOverlap.x;
////                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
//            }
//                // TOP SIDE COLLISION
//            else if (collisionPrevOverlap.x > 0 && playerPos.y < ePos.y) {
//                if(!m_player->getComponent<CInput>().canJump) {
//                    m_player->getComponent<CState>().state = StateType::IDLE;
//                    m_player->getComponent<CInput>().canJump = true;
//                }
//
//                m_player->getComponent<CTransform>().pos.y -= collisionOverlap.y;
//                m_playerConfig.gravityMultiplier = 1;
//            }
//                // BOTTOM SIDE COLLISION
//            else if (collisionPrevOverlap.x > 0 && playerPos.y > ePos.y) {
//                m_player->getComponent<CTransform>().pos.y += collisionOverlap.y;
//                m_player->getComponent<CTransform>().velocity.y = 0;
////                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
//            }
//        }

        // PLAYER ENEMY COLLISION (HEALTH AND INVINCIBILITY)
//        if (e->tag() == "enemy" && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
//            if(!m_player->hasComponent<CInvincibility>()) {
//                m_player->getComponent<CHealth>().remaining -= e->getComponent<CDamage>().damage;
//                m_player->addComponent<CInvincibility>(30);
//            }
//        }
    }
    // SWORD ENEMY COLLISION (HEALTH)
//    for (auto &sword: m_entityManager.getEntities("SWORD")) {
//        if (!(sword->hasComponent<CBoundingBox>())) { continue;};
//        for (auto &enemy: m_entityManager.getEntities("ENEMY")) {
//            if (!(enemy->hasComponent<CBoundingBox>())) { continue;};
//
//            auto& enemyBox = enemy->getComponent<CBoundingBox>();
//            auto& swordBox = sword->getComponent<CBoundingBox>();
//            // BOUNDING BOX
//            Vec2 collisionOverlap = m_physics.GetOverlap(sword, enemy);
//            if (collisionOverlap.x > 0 && collisionOverlap.y > 0) {
//                enemy->getComponent<CHealth>().remaining -= sword->getComponent<CDamage>().damage;
//                sword->getComponent<CDamage>().damage = 0;
//            }
//
//        }
//    }

}
void ScenePlay::sResolveTileCollision() {
    // possible soluiton to previous solution of having a vector of collision
    // in the collision, we check for overlap, we compute it again here
    for (auto &entity: m_entityManager.getEntities()) {
//        std::cout << entity->getComponent<CFrameCollision>().collisions.size() << std::endl;
        if(entity->hasComponent<CFrameCollision>()) {
            auto& collision = entity->getComponent<CFrameCollision>().collision;
            auto &entityPos = entity->getComponent<CTransform>().pos;
            auto &colliderPos = collision.collider->getComponent<CTransform>().pos;
            Vec2& collisionOverlap = collision.collisionOverlap;
            Vec2& previousCollisionOverlap = collision.previousCollisionOverlap;

            if (previousCollisionOverlap.y > 0 && entityPos.x > colliderPos.x) {
                entity->getComponent<CTransform>().pos.x += collisionOverlap.x;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
                // RIGHT SIDE COLLISION
            else if (previousCollisionOverlap.y > 0 && entityPos.x < colliderPos.x) {
                entity->getComponent<CTransform>().pos.x -= collisionOverlap.x;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
                // TOP SIDE COLLISION
            else if (previousCollisionOverlap.x > 0 && entityPos.y < colliderPos.y) {
                entity->getComponent<CTransform>().pos.y -= collisionOverlap.y;
                if(!entity->getComponent<CInput>().canJump) {
                    entity->getComponent<CState>().state = StateType::IDLE;
                    entity->getComponent<CInput>().canJump = true;
                    m_playerConfig.gravityMultiplier = 1;
                }
            }
                // BOTTOM SIDE COLLISION
            else if (previousCollisionOverlap.x > 0 && entityPos.y > colliderPos.y) {
                entity->getComponent<CTransform>().pos.y += collisionOverlap.y;
                entity->getComponent<CTransform>().velocity.y = 0;
            }
        }
        entity->getComponent<CFrameCollision>().has = false;
    }

}


void ScenePlay::sAnimation() {
    // UPDATE PLAYER ANIMATION
    auto &state = m_player->getComponent<CState>().state;
    auto &animation = m_player->getComponent<CAnimation>().animation;
    auto &input = m_player->getComponent<CInput>();
    auto& pTransform = m_player->getComponent<CTransform>();
    auto& velocity = pTransform.velocity;
    int direction{pTransform.direction};

    if (state == StateType::RUNNING && animation.getName() != "BlueRun") {
        auto& updatedAnimation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("BlueRun"));
        auto& sprite = updatedAnimation.animation.getSprite();
        sprite.setScale(direction * sprite.getScale().x, sprite.getScale().y);
    }
    else if (state == StateType::IDLE && animation.getName() != "Blue") {
        auto& updatedAnimation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("Blue"));
        auto& sprite = updatedAnimation.animation.getSprite();
        sprite.setScale( direction * sprite.getScale().x, sprite.getScale().y);
    }
    else if (state == StateType::ATTACKING && animation.getName() != "BlueAttack") {
        auto& updatedAnimation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("BlueAttack"));
        auto& sprite = updatedAnimation.animation.getSprite();
        sprite.setScale( direction * sprite.getScale().x, sprite.getScale().y);
    }
    else if (state == StateType::ATTACKING && animation.hasEnded()) {
        std::cout << "ended" << std::endl;
//        auto& updatedAnimation = m_player->addComponent<CAnimation>(m_assetManager.getAnimation("Blue"));
//        auto& sprite = updatedAnimation.animation.getSprite();
//        sprite.setScale( direction * sprite.getScale().x, sprite.getScale().y);
        state = StateType::IDLE;
    }

    // UPDATE ANIMATION TICK
    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CAnimation>()) {
            auto& animation = e->getComponent<CAnimation>().animation;
            // ADD TRANSPARENCY TO ENTITIES WITH IFRAMES
            if(e->hasComponent<CInvincibility>()) {
                animation.getSprite().setColor(sf::Color(255,255,255, 100));
            }
            else {
                animation.getSprite().setColor(sf::Color(255,255,255, 255));
            }
            // SIMULATE ANIMATION
            animation.update();
        }
    }

}

void ScenePlay::spawnSword() {
//    Vec2 &playerPos = m_player->getComponent<CTransform>().pos;
//    auto sword = m_entityManager.addEntity("SWORD");
////    sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos, Vec2(0, 0), 0);
//    sword->addComponent<CBoundingBox>(Vec2(64, 64), false, false);
//    sword->addComponent<CDamage>();
//    sword->addComponent<CLifespan>(20);
//
//    auto& pState = m_player->getComponent<CState>().state;
//    auto& pSize = m_player->getComponent<CAnimation>().animation.getSize();
//    if(pState == "STANDING_RIGHT" || pState == "RUNNING_RIGHT") {
//        sword->addComponent<CAnimation>(m_assetManager.getAnimation("SwordRight"));
//        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(64, 0), Vec2(0, 0), 0);
//    }
//    else if(pState == "STANDING_LEFT" || pState == "RUNNING_LEFT") {
//        sword->addComponent<CAnimation>(m_assetManager.getAnimation("SwordLeft"));
//        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(-64, 0), Vec2(0, 0), 0);
//    }
//    else if(pState == "STANDING_UP" || pState == "RUNNING_UP") {
//        sword->addComponent<CAnimation>(m_assetManager.getAnimation("SwordUp"));
//        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(0, -64), Vec2(0, 0), 0);
//    }
//    else if(pState == "STANDING_DOWN" || pState == "RUNNING_DOWN") {
//        sword->addComponent<CAnimation>(m_assetManager.getAnimation("Sword"));
//        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(0, 64), Vec2(0, 0), 0);
//    }
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
//    for (auto& e : m_entityManager.getEntities()) {
//        // UPDATE SWORD POSITION
//        if(e->tag() == "SWORD") {
//            auto& pState = m_player->getComponent<CState>().state;
//            auto& pPos = m_player->getComponent<CTransform>().pos;
//            if(pState == "STANDING_RIGHT" || pState == "RUNNING_RIGHT") {
//                e->getComponent<CTransform>().pos = pPos + Vec2(64, 0);
//            }
//            if(pState == "STANDING_LEFT" || pState == "RUNNING_LEFT") {
//                e->getComponent<CTransform>().pos = pPos + Vec2(-64, 0);
//            }
//            if(pState == "STANDING_UP" || pState == "RUNNING_UP") {
//                e->getComponent<CTransform>().pos = pPos + Vec2(0, -64);
//            }
//            if(pState == "STANDING_DOWN" || pState == "RUNNING_DOWN") {
//                e->getComponent<CTransform>().pos = pPos + Vec2(0, 64);
//            }
//        }
//    }

}

void ScenePlay::sHealth() {
    for(auto& e : m_entityManager.getEntities()) {
        if(!e->hasComponent<CHealth>()) { continue;}
        auto& health = e->getComponent<CHealth>().remaining;
        if(health < 1) {
            e->destroy();
            if(e->tag() == "player") {
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
//                Vec2 normalized = delta.normalize(delta.magnitude(delta));
                Vec2 normalized = delta.normalize();
                Vec2 velocity = normalized.scale(speed);
                enemy->getComponent<CTransform>().velocity = velocity;
            }
            else {
                float speed = enemy->getComponent<CFollowPlayer>().speed;
                Vec2 delta = enemy->getComponent<CFollowPlayer>().home - enemyPos;
                Vec2 normalized = delta.normalize();
                Vec2 velocity = normalized.scale(speed);
                if(delta.magnitude() <= 5) {
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
            Vec2 normalized = delta.normalize();
            Vec2 velocity = normalized.scale(speed);
            enemy->getComponent<CTransform>().velocity = velocity;

            if(delta.magnitude() <= 5) {
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
    if(e->hasComponent<CHitbox>()) {
//        auto& hitbox = e->getComponent<CHitbox>();
//        float direction = e->getComponent<CTransform>().direction;
//        sf::RectangleShape box(sf::Vector2f(hitbox.size.x, hitbox.size.y));
//        box.setPosition((pos.x + (hitbox.offset.x * direction)), pos.y + hitbox.offset.y);
//        box.setOrigin(hitbox.size.x/2, hitbox.size.y/2);
//        box.setFillColor(sf::Color(230, 10, 90, 110));
//        box.setOutlineThickness(2);
//        box.setOutlineColor(sf::Color(255, 255, 255));
//        m_game->draw(box);
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
            auto& eVelocity = e->getComponent<CTransform>().velocity;
            bool eCanJump = e->getComponent<CInput>().canJump;
            // increasing gravity when player is falling from the apex of their jump
            if(eVelocity.y > 5 && !eCanJump) {
                m_playerConfig.gravityMultiplier = 1.5f;
            }
            // increase hang time
            if(eVelocity.y > -3 && eVelocity.y < 3 && !eCanJump) {
                m_playerConfig.gravityMultiplier = 0.7f;
            }
            float newGravity = e->getComponent<CTransform>().velocity.y + (gravity * m_playerConfig.gravityMultiplier);
            e->getComponent<CTransform>().velocity.y = std::min(newGravity, m_playerConfig.MAXSPEED);
        }
        e->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y;
    }
}

void ScenePlay::setEntityState(std::shared_ptr<Entity> e, StateType state) {
    e->getComponent<CState>().state = state;
}

void ScenePlay::sDialogue() {
    for (auto &e: m_entityManager.getEntities()) {
        auto& dialogue = e->getComponent<CDialogue>();
        if(!e->hasComponent<CDialogue>() || !dialogue.active) { continue; }

        float width{600}, height{200}, posX{(float)m_game->window().getSize().x/2}, posY{height/2}, marginX{30}, marginY{30};
        sf::RectangleShape textBox(sf::Vector2f(width, height));
        textBox.setPosition(posX,posY + marginY);
        textBox.setOrigin(width/2, height/2);
        textBox.setFillColor(sf::Color::Black);
        sf::Text sentence(dialogue.dialogues.front().substr(0, dialogue.currentCharIdx), m_assetManager.getDefaultFont(), 18);
        sf::Text speakerName(dialogue.npcName, m_assetManager.getDefaultFont(), 24);

        if(m_currentFrame % 6 == 0) {
            dialogue.currentCharIdx++;
        }
        sentence.setPosition(posX + marginX - (width / 2), posY + (marginY * 2) - (height / 2));
        speakerName.setPosition(marginX, m_game->window().getSize().y - speakerName.getGlobalBounds().getSize().y - marginY);
        m_game->window().draw(textBox);
        m_game->window().draw(sentence);
        m_game->window().draw(speakerName);

        // resets the active status,collision will set it to true if the two entities still overlap
        dialogue.active = false;
    }
}


void ScenePlay::drawTextEnvironementInformation(std::string text) {
    float posX{0}, posY, textSize{24};
    float marginX{30}, marginY{-30};
    sf::Color fill{sf::Color::White};
    auto e = m_entityManager.addEntity("text");
    e->addComponent<CLifespan>(100);
    auto& textEntity = e->addComponent<CText>().text;
    textEntity.setString(text);
    textEntity.setCharacterSize(textSize);
    textEntity.setFont(m_assetManager.getDefaultFont());
    textEntity.setFillColor(fill);
    posY = m_game->window().getSize().y - textEntity.getGlobalBounds().getSize().y;

    textEntity.setPosition(posX + marginX, posY + marginY);
}

void ScenePlay::sIFrames() {
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



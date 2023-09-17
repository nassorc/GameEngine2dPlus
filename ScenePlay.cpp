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

ScenePlay::ScenePlay(GameEngine *gameEngine, const std::string &levelPath) : Scene(gameEngine) {
    m_entityManager = EntityManager();
    init(levelPath);
}

void ScenePlay::init(const std::string &levelPath) {
    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::A, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");
    registerAction(sf::Keyboard::Key::Space, "ATTACK");
    registerAction(sf::Keyboard::Key::P, "TOGGLE_PAUSE");
    registerAction(sf::Keyboard::Key::C, "SWITCH_CAMERA_MODE");
    registerAction(sf::Keyboard::Key::V, "TOGGLE_DRAW_COLLISIONS");
    registerAction(sf::Keyboard::Key::T, "TOGGLE_DRAW_TEXTURES");
    loadLevel(levelPath);
}

void ScenePlay::onEnd() {

}

void ScenePlay::resetPlayerInput() {
    m_player->getComponent<CInput>().up = false;
    m_player->getComponent<CInput>().down = false;
    m_player->getComponent<CInput>().left = false;
    m_player->getComponent<CInput>().right = false;
}

void ScenePlay::sDoAction(const Action &action) {
    if (action.type() == "START") {
        if (action.name() == "UP") {
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
                spawnSword();
            }
        } else if (action.name() == "TOGGLE_PAUSE") {
            m_paused = !m_paused;
        } else if (action.name() == "SWITCH_CAMERA_MODE") {
            m_camera = (m_camera == Camera::Room) ? Camera::Follow : Camera::Room;
        } else if (action.name() == "TOGGLE_DRAW_COLLISIONS") {
            m_drawCollisions = !m_drawCollisions;
        } else if (action.name() == "TOGGLE_DRAW_TEXTURES") {
            m_drawTextures = !m_drawTextures;
        }
    }
    if (action.type() == "END") {
        if (action.name() == "UP") m_player->getComponent<CInput>().up = false;
        else if (action.name() == "DOWN") m_player->getComponent<CInput>().down = false;
        else if (action.name() == "LEFT") m_player->getComponent<CInput>().left = false;
        else if (action.name() == "RIGHT") m_player->getComponent<CInput>().right = false;
    }

}

void ScenePlay::sRender() {
    m_entityManager.update();

    if (!m_paused) { m_game->window().clear(sf::Color(252, 216, 168)); }
    else { m_game->window().clear(sf::Color(240, 198, 156)); }
    // DRAW ENTITIES
    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CAnimation>()) {
            auto &eTransform = e->getComponent<CTransform>();
            e->getComponent<CAnimation>().animation.getSprite().setPosition(eTransform.pos.x, eTransform.pos.y);
            if (m_drawTextures) {
                auto &sprite = e->getComponent<CAnimation>().animation.getSprite();
                m_game->window().draw(sprite);
            }

            //DEBUG DRAW BOUNDING BOXES
            if (m_drawCollisions && e->hasComponent<CBoundingBox>()) {
                auto &pos = e->getComponent<CAnimation>().animation.getSprite().getPosition();
                auto &size = e->getComponent<CBoundingBox>().size;
                sf::RectangleShape box(sf::Vector2f(size.x, size.y));
                box.setOrigin(size.x / 2, size.y / 2);
                box.setPosition(pos.x, pos.y);
                box.setFillColor(sf::Color(255, 255, 255, 0));
                box.setOutlineThickness(2);
                box.setOutlineColor(sf::Color(255, 255, 255));
                m_game->window().draw(box);

                // DRAW ENEMY POINTS
                sf::CircleShape point(6);
                point.setOrigin(3, 3);
                point.setPosition(pos.x, pos.y);
                point.setFillColor(sf::Color(0, 0, 0));
                m_game->window().draw(point);

                // DRAW AI PATROL POINTS
                if(e->hasComponent<CPatrol>()) {
                    for(auto& p : e->getComponent<CPatrol>().positions) {
                        sf::CircleShape point(6);
                        point.setOrigin(3, 3);
                        point.setPosition(p.x, p.y);
                        point.setFillColor(sf::Color(0, 0, 0));
                        m_game->window().draw(point);

                    }
                }
            }

            if (m_drawCollisions && e->hasComponent<CFollowPlayer>()) {
                auto& p1 = e->getComponent<CTransform>().pos;
                auto& p2 = m_player->getComponent<CTransform>().pos;
                drawLine(p1, p2);
            }

            // ADD HEALTH BAR
            if(e->hasComponent<CHealth>()) {
                auto& ePos = e->getComponent<CTransform>().pos;
                auto& health = e->getComponent<CHealth>();
                float healthRatio = (float) health.remaining / (float) health.total;

                sf::RectangleShape healthBackground(sf::Vector2f{ 64, 8});
                healthBackground.setOrigin(32, 32);
                healthBackground.setPosition(ePos.x, ePos.y- 20);
                healthBackground.setFillColor(sf::Color::Black);
                m_game->window().draw(healthBackground);

                sf::RectangleShape healthBar(sf::Vector2f{healthRatio * 64, 6});
                healthBar.setOrigin(32, 32);
                healthBar.setPosition(ePos.x, ePos.y- 20);
                healthBar.setFillColor(sf::Color::Red);
                m_game->window().draw(healthBar);
            }
        }
    }
    //DEBUG DRAW BOUNDING BOXES
//    if (m_drawCollisions) {
//        for (auto &e: m_entityManager.getEntities()) {
//            if (e->hasComponent<CBoundingBox>()) {
//                auto &pos = e->getComponent<CAnimation>().animation.getSprite().getPosition();
//                auto &size = e->getComponent<CBoundingBox>().size;
//                sf::RectangleShape box(sf::Vector2f(size.x, size.y));
//                box.setOrigin(size.x / 2, size.y / 2);
//                box.setPosition(pos.x, pos.y);
//                box.setFillColor(sf::Color(255, 255, 255, 0));
//                box.setOutlineThickness(2);
//                box.setOutlineColor(sf::Color(255, 255, 255));
//                m_game->window().draw(box);
//            }
//        }
//    }

    // DISPLAY ENTITIES
    m_game->window().display();
}

void ScenePlay::update() {
    sAI();
    sMovement();
    sAttack();
    sHealth();
    sLifespan();
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
    std::ifstream fin(levelPath);
    if (!fin) {
        std::cerr << "Could open level path: " << levelPath << std::endl;
        exit(-1);
    }

    std::string type, aniName;
    int rx, ry, x, y, bx, by, maxLife, speed, damage;
    bool blocksVision, blocksMovement;

    while (fin >> type) {
        // skip comment line
        if (type == "#") getline(fin, type, '\n');
        std::transform(type.begin(), type.end(), type.begin(), ::toupper);

        if (type == "TILE") {
            fin >> aniName >> rx >> ry >> x >> y >> blocksMovement >> blocksVision;
            auto tile = m_entityManager.addEntity("TILE");
            // convert screen position to world coordinates
            tile->addComponent<CTransform>(getPosition(Vec2(rx, ry), Vec2(x, y)), Vec2(0, 0), 0);
            auto &tileAnimation = tile->addComponent<CAnimation>(
                    m_game->getAssets().getAnimation(aniName)).animation.getSprite();
            tile->addComponent<CBoundingBox>(Vec2(64, 64), blocksMovement, blocksVision);
        }
        else if (type == "PLAYER") {
            fin >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.BX >> m_playerConfig.BY >> m_playerConfig.S
                >> m_playerConfig.ML;
            fin >> x >> y >> bx >> by >> speed >> maxLife;
        }
        else if (type == "NPC") {
            int numberOfPatrolPositions;
            std::string AIBehavior;
            fin >> aniName >> rx >> ry >> x >> y >> blocksMovement >> blocksVision >> maxLife >> damage >> AIBehavior >> speed;

            auto enemy = m_entityManager.addEntity("ENEMY");
            Vec2 worldPos =  getPosition(Vec2(rx, ry), Vec2(x, y));
            enemy->addComponent<CTransform>(worldPos, Vec2(0, 0), 0);
            enemy->addComponent<CAnimation>(m_game->getAssets().getAnimation(aniName)).animation.getSprite();
            enemy->addComponent<CBoundingBox>(Vec2(64, 64), blocksMovement, blocksVision);
            enemy->addComponent<CDamage>(damage);
            enemy->addComponent<CHealth>(maxLife, maxLife);

            if (AIBehavior == "Patrol") {
                std::vector<Vec2> positions;
                int n;
                float patrolX, patrolY;
                fin >> n;
                while (n > 0) {
                    fin >> patrolX >> patrolY;
                    positions.push_back(getPosition(Vec2(rx, ry), Vec2(patrolX, patrolY)));
                    n--;
                }
                enemy->addComponent<CPatrol>(positions, speed);
            }
            else {
                enemy->addComponent<CFollowPlayer>(worldPos, speed);
            }
        }
    }
    // spawn player on level
    spawnPlayer();
}

void ScenePlay::sMovement() {
    // PLAYER MOVEMENT
    Vec2 velocity = {0.0f, 0.0f};
    auto &input = m_player->getComponent<CInput>();
    auto& state = m_player->getComponent<CState>().state;

    if (input.up) {
        state = "RUNNING_UP";
        velocity.y = m_playerConfig.S * -1;
    } else if (input.down) {
        state = "RUNNING_DOWN";
        velocity.y = m_playerConfig.S;
    } else if (input.left) {
        state = "RUNNING_LEFT";
        velocity.x = m_playerConfig.S * -1;
    } else if (input.right) {
        state = "RUNNING_RIGHT";
        velocity.x = m_playerConfig.S;
    }
    else if (!input.up && state == "RUNNING_UP") {
        state = "STANDING_UP";
    }
    else if (!input.down && state == "RUNNING_DOWN") {
        state = "STANDING_DOWN";
    }
    else if (!input.left && state == "RUNNING_LEFT") {
        state = "STANDING_LEFT";
    }
    else if (!input.right && state == "RUNNING_RIGHT") {
        state = "STANDING_RIGHT";
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
    auto e = m_entityManager.addEntity("PLAYER");
    e->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y), Vec2(0, 0), 0);
    e->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"));
    e->addComponent<CBoundingBox>(Vec2(m_levelConfig.TILE_WIDTH, m_levelConfig.TILE_WIDTH), false, false);
    e->addComponent<CHealth>(m_playerConfig.ML, m_playerConfig.ML);
    e->addComponent<CInvincibility>(30);
    e->addComponent<CInput>();
    e->addComponent<CState>();
    m_player = e;
}

void ScenePlay::sCamera() {
    sf::Vector2u windowSize = m_game->window().getSize();
    m_view = sf::View(sf::FloatRect(0.f, 0.f, (float) windowSize.x, (float) windowSize.y));
    if (m_camera == Camera::Follow) {
        auto &playerPos = m_player->getComponent<CTransform>().pos;
        m_view.move(sf::Vector2f(playerPos.x - ((float) windowSize.x / 2), playerPos.y - ((float) windowSize.y / 2)));
        m_game->window().setView(m_view);
    } else if (m_camera == Camera::Room) {
        auto &playerPos = m_player->getComponent<CTransform>().pos;
        Vec2 roomPos = {(float) ((int) playerPos.x / (int) windowSize.x),
                        (float) ((int) playerPos.y / (int) windowSize.y)};
        if (playerPos.x < 0) roomPos.x -= 1;
        if (playerPos.y < 0) roomPos.y -= 1;
        m_view.setCenter((windowSize.x) * roomPos.x + (windowSize.x / 2),
                         (windowSize.y) * roomPos.y + (windowSize.y / 2));
//        m_view.move(sf::Vector2f(playerPos.x - ((float)windowSize.x / 2), playerPos.y - ((float) windowSize.y / 2)));
        m_game->window().setView(m_view);
    }
}

Vec2 ScenePlay::getPosition(const Vec2 &room, const Vec2 &tile) {
    const int TILE_WIDTH = 64;
    const int TILE_HWIDTH = 32;
    sf::Vector2u windowSize = m_game->window().getSize();
    Vec2 pos(room.x * (float) windowSize.x + tile.x * TILE_WIDTH + TILE_HWIDTH,
             room.y * (float) windowSize.y + tile.y * TILE_WIDTH + TILE_HWIDTH);

//    Vec2 worldCoords(coords.x * 64 + 32, coords.y * 64 + 32);
//    return worldCoords;
    return pos;
}

void ScenePlay::sCollision() {
    // PLAYER TILE COLLISION
    for (auto &e: m_entityManager.getEntities()) {
        if (!(e->hasComponent<CBoundingBox>())) { continue;}
        if (e->tag() == "PLAYER") { continue;}

        // BOUNDING BOX
        auto& bBox = e->getComponent<CBoundingBox>();

        // TILE PLAYER BOUNDING BOX COLLISION
        Vec2 collisionOverlap = m_physics.GetOverlap(m_player, e);
        if (e->tag() == "TILE" && bBox.blockMovement && collisionOverlap.x > 0 && collisionOverlap.y > 0) {
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
                m_player->getComponent<CTransform>().pos.y -= collisionOverlap.y;
//                m_player->getComponent<CTransform>().velocity = Vec2(0, 0);
            }
                // BOTTOM SIDE COLLISION
            else if (collisionPrevOverlap.x > 0 && playerPos.y > ePos.y) {
                m_player->getComponent<CTransform>().pos.y += collisionOverlap.y;
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

    if (state == "STANDING_DOWN")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"));
    else if (state == "STANDING_UP")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("StandUp"));
    else if (state == "STANDING_LEFT")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("StandLeft"));
    else if (state == "STANDING_RIGHT")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("StandRight"));
    else if (state == "RUNNING_UP" && animation.getName() != "RunUp")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("RunUp"));
    else if (state == "RUNNING_DOWN" && animation.getName() != "RunDown")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("RunDown"));
    else if (state == "RUNNING_LEFT" && animation.getName() != "RunLeft")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("RunLeft"));
    else if (state == "RUNNING_RIGHT" && animation.getName() != "RunRight")
        m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("RunRight"));
    // update animation and animation position
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
        sword->addComponent<CAnimation>(m_game->getAssets().getAnimation("SwordRight"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(64, 0), Vec2(0, 0), 0);
    }
    else if(pState == "STANDING_LEFT" || pState == "RUNNING_LEFT") {
        sword->addComponent<CAnimation>(m_game->getAssets().getAnimation("SwordLeft"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(-64, 0), Vec2(0, 0), 0);
    }
    else if(pState == "STANDING_UP" || pState == "RUNNING_UP") {
        sword->addComponent<CAnimation>(m_game->getAssets().getAnimation("SwordUp"));
        sword->addComponent<CTransform>(m_player->getComponent<CTransform>().pos + Vec2(0, -64), Vec2(0, 0), 0);
    }
    else if(pState == "STANDING_DOWN" || pState == "RUNNING_DOWN") {
        sword->addComponent<CAnimation>(m_game->getAssets().getAnimation("Sword"));
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



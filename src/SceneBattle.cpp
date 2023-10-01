//
// Created by Matthew Crossan on 9/26/23.
//

#include "SceneBattle.h"

SceneBattle::SceneBattle(GameEngine *gameEngine, Assets &assetManager) : Scene(gameEngine, assetManager) {
    init();
}

void SceneBattle::init() {
    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::A, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");

    spawnTile(Vec2(5, 2));
    spawnTile(Vec2(6, 2));
    spawnTile(Vec2(7, 2));
    spawnTile(Vec2(8, 2));
    spawnTile(Vec2(9, 2));
    spawnTile(Vec2(10, 2));
    spawnTile(Vec2(11, 2));
    spawnTile(Vec2(5, 3));
    spawnTile(Vec2(5, 4));
    spawnTile(Vec2(5, 5));
    spawnTile(Vec2(5, 6));

    spawnInteractable(Vec2(7, 1));

    spawnPlayer();
}

void SceneBattle::update() {
    m_entityManager.update();
    sPlayerInput();
    sMovement();
    sCollision();
    sTileCollision();
    sHitDetection();
    sGravity();
    sRender();
    // cleanup
    sCollisionCleanup();
}

void SceneBattle::sDoAction(const Action &action) {
    if (action.type() == "START") {
        if (action.name() == "UP") {
            m_player->getComponent<CInput>().up = true;
        } else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = true;
        } else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = true;
        } else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = true;
        }
    }
    if(action.type() == "END") {
        if (action.name() == "UP") {
            m_player->getComponent<CInput>().up = false;
        } else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = false;
        } else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = false;
        } else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = false;
        }
    }
}

void SceneBattle::sMovement() {
    // update position
    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CTransform>()) {
            auto &transform = e->getComponent<CTransform>();
            transform.pos += transform.velocity;
        }
    }
}

void SceneBattle::sPlayerInput() {
    Vec2 velocity = {0, 0};
    auto& transform = m_player->getComponent<CTransform>();
    auto &input = m_player->getComponent<CInput>();

    if (input.up) {
        velocity.y = -m_playerConfig.speed;
    }
    if(input.down) {
        velocity.y = m_playerConfig.speed;
    }
    if (input.left) {
        velocity.x = -m_playerConfig.speed;
    }
    if (input.right) {
        velocity.x = m_playerConfig.speed;
    }
    m_player->getComponent<CTransform>().velocity = velocity;

    // SET PREVIOUS POSITION
    m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos;

}

void SceneBattle::sRender() {
    m_game->window().clear(sf::Color::Black);
    for(auto& e: m_entityManager.getEntities()) {
        if (!e->hasComponent<CRect>()) { continue; }
        auto& rect = e->getComponent<CRect>().rect;
        auto& newPos = e->getComponent<CTransform>().pos;
        rect.setPosition(newPos.x, newPos.y);
        m_game->draw(rect);
    }
}

void SceneBattle::sCollision() {
    for (auto &e1: m_entityManager.getEntities()) {
        if(!e1->hasComponent<CBoundingBox>() || !e1->hasComponent<CMovable>()) { continue; }
        for (auto &e2: m_entityManager.getEntities()) {
            if(e1->tag() == e2->tag()) { continue; }
            if(!e1->hasComponent<CBoundingBox>()) { continue; }

            Vec2 collisionOverlap = m_physics.GetOverlap(e1, e2);

            auto& e1Pos = e1->getComponent<CTransform>().pos;
            auto& e2Pos = e2->getComponent<CTransform>().pos;

            if(collisionOverlap.x > 0 && collisionOverlap.y > 0) {
                Vec2 collisionPrevOverlap = m_physics.GetPreviousOverlap(e1, e2);
                if(e1->hasComponent<CCollide>()) {
                    e1->getComponent<CCollide>().addCollision(e2, collisionOverlap, collisionPrevOverlap);
                }
                else {
                    e1->addComponent<CCollide>(e2, collisionOverlap, collisionPrevOverlap);
                }
            }
        }
    }
}

void state() {

}

void SceneBattle::sTileCollision() {
    std::map<size_t, Vec2> collisionMap;
    for (auto &e1: m_entityManager.getEntities()) {
        if(!e1->hasComponent<CCollide>()) { continue; }

        for(auto& collision : e1->getComponent<CCollide>().collisions) {
            auto& overlap = collision.collisionOverlap;
            auto& previousOverlap = collision.previousCollisionOverlap;
            auto e2 = collision.collider;

            if(!e2->hasComponent<CBoundingBox>() || !e2->getComponent<CBoundingBox>().blockMovement) { continue; }

            auto& e1Pos = e1->getComponent<CTransform>().pos;
            auto& e2Pos = e2->getComponent<CTransform>().pos;

            if (previousOverlap.x > 0 && e1Pos.y < e2Pos.y) {
                collisionMap[e1->id()].y = -1 * overlap.y;
                collision.type = Collision::CollisionType::TOP;
            }
            else if (previousOverlap.x > 0 && e1Pos.y > e2Pos.y) {
                collisionMap[e1->id()].y = overlap.y;
                collision.type = Collision::CollisionType::BOTTOM;
            }
            else if (previousOverlap.y > 0 && e1Pos.x < e2Pos.x) {
                collisionMap[e1->id()].x = -1 * overlap.x;
                collision.type = Collision::CollisionType::LEFT;
            }
            else if (previousOverlap.y > 0 && e1Pos.x > e2Pos.x) {
                collisionMap[e1->id()].x = overlap.x;
                collision.type = Collision::CollisionType::RIGHT;
            }
        }
    }

    for (auto &e1: m_entityManager.getEntities()) {
        if(collisionMap.find(e1->id()) == collisionMap.end()) { continue; }
        e1->getComponent<CTransform>().pos.x += collisionMap[e1->id()].x;
        e1->getComponent<CTransform>().pos.y += collisionMap[e1->id()].y;
    }
}

void SceneBattle::sHitDetection() {
    for (auto &e1: m_entityManager.getEntities()) {
        if(!e1->hasComponent<CCollide>()) { continue; }
        if(!e1->hasComponent<CHealth>()) { continue; }

        for(auto& collision : e1->getComponent<CCollide>().collisions) {
            auto e2 = collision.collider;

            if(!e2->hasComponent<CBoundingBox>()) { continue; }
            if(!e2->hasComponent<CDamage>()) { continue; }
        }
    }
}

void SceneBattle::sInteract() {
}

void SceneBattle::sGravity() {
    for (auto &e1: m_entityManager.getEntities()) {
        for(auto& collision : e1->getComponent<CCollide>().collisions) {
            if(collision.type == Collision::CollisionType::TOP) {
                std::cout << "grounded" << std::endl;
            }
        }
    }

}

void SceneBattle::sAnimation() {

}

void SceneBattle::sState() {
    for (auto &e1: m_entityManager.getEntities()) {
        for(auto& collision : e1->getComponent<CCollide>().collisions) {
            if(collision.type == Collision::CollisionType::TOP) { // can jump
                e1->getComponent<CState>().state = StateType::IDLE;
            }
        }
    }
}

void SceneBattle::sCollisionCleanup() {
    for (auto &e1: m_entityManager.getEntities()) {
        if(!e1->hasComponent<CCollide>()) { continue; }
        e1->getComponent<CCollide>().has = false;
    }
}



void SceneBattle::onEnd() {

}

void SceneBattle::spawnPlayer() {
    auto player = m_entityManager.addEntity("player");

    player->addComponent<CTransform>(Vec2{(float) m_playerConfig.x, (float)m_playerConfig.y}, Vec2{0,0}, 0);
    player->addComponent<CBoundingBox>(Vec2{(float) m_playerConfig.width, (float)m_playerConfig.height}, false, false);
    player->addComponent<CInput>();
    player->addComponent<CMovable>();
    player->addComponent<CState>();
    player->addComponent<CHealth>(10);

    auto& rect = player->addComponent<CRect>(Vec2(m_playerConfig.width, m_playerConfig.height)).rect;
    rect.setPosition(m_playerConfig.x, m_playerConfig.y);
    rect.setFillColor(m_playerConfig.fc);
    m_player = player;
}

void SceneBattle::spawnTile(const Vec2 &tilePos) {
    auto tile = m_entityManager.addEntity("tile");

    Vec2 adjustedPos{tilePos.x * (float) m_worldConfig.tileSize, tilePos.y * (float) m_worldConfig.tileSize};

    tile->addComponent<CTransform>(adjustedPos, Vec2{0, 0}, 0);
    tile->addComponent<CInput>();
    tile->addComponent<CBoundingBox>(Vec2{(float) m_worldConfig.tileWidth, (float) m_worldConfig.tileHeight}, true, true);

    auto& rect = tile->addComponent<CRect>(Vec2(m_worldConfig.tileWidth, m_worldConfig.tileHeight)).rect;
    rect.setPosition(adjustedPos.x, adjustedPos.y);
    rect.setFillColor(m_worldConfig.fc);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::White);

}

void SceneBattle::spawnInteractable(const Vec2 &tilePos) {
    auto tile = m_entityManager.addEntity("npc");

    Vec2 adjustedPos{tilePos.x * (float) m_worldConfig.tileSize, tilePos.y * (float) m_worldConfig.tileSize};

    tile->addComponent<CDamage>(5);
    tile->addComponent<CTransform>(adjustedPos, Vec2{0, 0}, 0);
    tile->addComponent<CBoundingBox>(Vec2{(float) m_worldConfig.tileWidth, (float) m_worldConfig.tileHeight}, false, false);

    auto& rect = tile->addComponent<CRect>(Vec2(m_worldConfig.tileWidth, m_worldConfig.tileHeight)).rect;
    rect.setPosition(adjustedPos.x, adjustedPos.y);
    rect.setFillColor(sf::Color::Green);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::White);
}


Vec2 SceneBattle::calculateWorldPos() {
}


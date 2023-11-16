//
// Created by Matthew Crossan on 9/26/23.
//

#include "SceneBattle.h"
#include "../core/CoreManager.h"
#include "../components/Components.h"
#include "../include/System.h"
#include "../core/World.h"
#include "../systems/PlayerInputSystem.h"
#include "../systems/MovementSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/GravitySystem.h"
#include "../systems/CleanupSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/HitboxSystem.h"
#include "../systems/LifespanSystem.h"
#include "../systems/PlayerStateSystem.h"
#include "../systems/AISystem.h"
#include "../systems/SpellCasterSystem.h"
#include "../systems/DialogueSystem.h"

#include "../utils/ImageLevelLoader1.h"
#include "../utils/TilesetLevelLoader1.h"
#include "../utils/WorldObjectsLevelLoader1.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

SceneBattle::SceneBattle(GameEngine *gameEngine, Assets &assetManager) : Scene(gameEngine, assetManager) {
    init();
}

//CoreManager world;
PlayerConfig g_playerConfig;

extern std::shared_ptr<Entity1> g_selectedEntity;
extern World world;

void SceneBattle::init() {
    g_playerConfig = m_playerConfig;
//    world.init();
//    world.coreManager = &world;
//    world.coreManager = new CoreManager();
//    world.coreManager->init();
    // register components
    world.registerComponent<CTransform>();
    world.registerComponent<CInput>();
    world.registerComponent<CRect>();
    world.registerComponent<CBoundingBox>();
    world.registerComponent<CMovable>();
    world.registerComponent<CState>();
    world.registerComponent<CHealth>();
    world.registerComponent<CDamage>();
    world.registerComponent<CHitbox>();
    world.registerComponent<CCollide1>();
    world.registerComponent<CLifespan>();
    world.registerComponent<CAnimation>();
    world.registerComponent<CAI>();
    world.registerComponent<CAISeekBehavior>();
    world.registerComponent<CAIFlying>();
    world.registerComponent<CSpellCaster>();
    world.registerComponent<CDialogue1>();
    world.registerComponent<CPlayer>();

    // register systems and set it's signature

    world.registerSystem<PlayerInputSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CTransform>());
        signature.set(world.getComponentType<CInput>());
        world.setSystemSignature<PlayerInputSystem>(signature);
    }
    world.registerSystem<MovementSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CTransform>());
        world.setSystemSignature<MovementSystem>(signature);
    }
    world.registerSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CBoundingBox>());
        world.setSystemSignature<PhysicsSystem>(signature);
    }
    world.registerSystem<DamageSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CCollide1>());
        world.setSystemSignature<DamageSystem>(signature);
    }
//    world.registerSystem<GravitySystem>();
//    {
//        Signature signature;
//        signature.set(world.getComponentType<CGravity>());
//        signature.set(world.getComponentType<CTransform>());
//        world.setSystemSignature<GravitySystem>(signature);
//    }
    world.registerSystem<RenderSystem>();
    {
        Signature signature;
//        signature.set(world.getComponentType<CRect>());
        signature.set(world.getComponentType<CTransform>());
        world.setSystemSignature<RenderSystem>(signature);
    }
    world.registerSystem<CleanupSystem>();
    {
        Signature signature;
        world.setSystemSignature<CleanupSystem>(signature);
    }
    world.registerSystem<HitboxSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CHitbox>());
        world.setSystemSignature<HitboxSystem>(signature);
    }
    world.registerSystem<LifespanSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CLifespan>());
        world.setSystemSignature<LifespanSystem>(signature);
    }
    world.registerSystem<PlayerStateSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CState>());
        world.setSystemSignature<PlayerStateSystem>(signature);
    }
    world.registerSystem<AISystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CAI>());
        world.setSystemSignature<AISystem>(signature);
    }
    world.registerSystem<SpellCasterSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CSpellCaster>());
        signature.set(world.getComponentType<CState>());
        world.setSystemSignature<SpellCasterSystem>(signature);
    }
    world.registerSystem<DialogueSystem>();
    {
        Signature signature;
        signature.set(world.getComponentType<CBoundingBox>());
        signature.set(world.getComponentType<CCollide1>());
        world.setSystemSignature<DialogueSystem>(signature);
    }

    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::A, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");
    registerAction(sf::Keyboard::Key::Space, "ATTACK");

    std::vector<std::string> level = {
            "0000000000000000000",
            "00X000000000000E000",
            "00X0000I00000000000",
            "00X0000000X00000000",
            "00X0000000000000F00",
            "00X0000000000000000",
            "0000000X00000000000",
            "0000000000000000000",
            "000000000000000I000",
            "0000000000XXXXXXXXX",
            "XXXXXXXXXXXXXXXXXXX"
            "XXXXXXXXXXXXXXXXXXX"
    };

    m_levelLoaderMap["imagelayer"] = new ImageLevelLoader1();
    m_levelLoaderMap["tilelayer"] = new TilesetLevelLoader1();
    m_levelLoaderMap["objectgroup"] = new WorldObjectsLevelLoader1();

    loadLevel(level);
    spawnPlayer();
}

void SceneBattle::loadLevel(std::vector<std::string> levelArr) {
    // world.levelLoader("./config")
    //     will handle with the json parse stuff
    //     talks with the asset manager

    // tileset.load("level1")
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
}

void SceneBattle::update() {
    world.coreManager->update();
    world.coreManager->getSystem<PlayerInputSystem>()->update();
    world.coreManager->getSystem<LifespanSystem>()->update();
    world.coreManager->getSystem<MovementSystem>()->update();
    world.coreManager->getSystem<PhysicsSystem>()->update();
    world.coreManager->getSystem<PlayerStateSystem>()->update();
    world.coreManager->getSystem<SpellCasterSystem>()->update();
    world.coreManager->getSystem<DamageSystem>()->update();
    world.coreManager->getSystem<HitboxSystem>()->update();
    world.coreManager->getSystem<DialogueSystem>()->update();
    world.coreManager->getSystem<AISystem>()->update();
//    world.getSystem<GravitySystem>()->update();
    world.coreManager->getSystem<RenderSystem>()->update();
    world.coreManager->getSystem<CleanupSystem>()->update();
}

void SceneBattle::sDoAction(const Action &action) {
    if (action.type() == "START") {
        if (action.name() == "UP") {
            world.get<CInput>(m_player1).up = true;
        } else if (action.name() == "DOWN") {
            world.get<CInput>(m_player1).down = true;
        } else if (action.name() == "LEFT") {
            world.get<CInput>(m_player1).left = true;
        } else if (action.name() == "RIGHT") {
            world.get<CInput>(m_player1).right = true;
        } else if (action.name() == "ATTACK") {
            world.get<CInput>(m_player1).attack = true;
        }

        // mouse event
        if(action.name() == "MOUSE_CLICK") {
            world.get<CState>(m_player1).state = StateType::ATTACKING;

            // ALLOWS THE LEVEL EDITOR TO ACCESS SELECTED ITEM
            for(auto& e : world.getEntities()) {
                if(!world.has<CTransform>(e) || !world.has<CBoundingBox>(e)) { continue; }
                auto mPos = action.pos();
                auto ePos = world.get<CTransform>(e).pos;
                Vec2 delta{std::abs(mPos.x - ePos.x), std::abs(mPos.y - ePos.y)};
                if(delta.x < 32 && delta.y < 32) {
                    g_selectedEntity = e;
                }
            }
        }
    }
    if(action.type() == "END") {
        if (action.name() == "UP") {
            world.get<CInput>(m_player1).up = false;
        } else if (action.name() == "DOWN") {
            world.get<CInput>(m_player1).down = false;
        } else if (action.name() == "LEFT") {
            world.get<CInput>(m_player1).left = false;
        } else if (action.name() == "RIGHT") {
            world.get<CInput>(m_player1).right = false;
        }
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

void SceneBattle::sState() {
    for (auto &e1: m_entityManager.getEntities()) {
        for(auto& collision : e1->getComponent<CCollide>().collisions) {
            if(collision.type == Collision::CollisionType::TOP) { // can jump
                e1->getComponent<CState>().state = StateType::IDLE;
            }
        }
    }
}

void SceneBattle::onEnd() {

}

void SceneBattle::spawnPlayer() {
    auto p = world.createEntity("player");
    Vec2 spawnPos{(float) m_playerConfig.x, (float)m_playerConfig.y};
    Vec2 size{(float) m_playerConfig.width, (float)m_playerConfig.height};
    world.set<CTransform>(p, spawnPos, Vec2{0.f, 0.f}, 0.f);
    world.set<CInput>(p);
    world.set<CBoundingBox>(p, size, false, false);
    world.set<CMovable>(p);
    world.set<CSpellCaster>(p, SpellType::FIREBALL, p);
//    world.addComponent(p, CHitbox{    p, Vec2(m_playerConfig.width, m_playerConfig.height), Vec2(m_playerConfig.width, 0), 1, true      });
    world.set<CRect>(p, Vec2(m_playerConfig.width, m_playerConfig.height));
    world.set<CState>(p);
    world.set<CHealth>(p, 10);
    world.set<CPlayer>(p);

    m_player1 = p;
}

void SceneBattle::spawnTile(const Vec2 &tilePos) {
    Vec2 adjustedPos{tilePos.x * (float) m_worldConfig.tileSize, tilePos.y * (float) m_worldConfig.tileSize};

    auto tile1 = world.createEntity("tile");
    world.set<CTransform>(tile1, adjustedPos, Vec2{0.f, 0.f}, 0.f);
    world.set<CBoundingBox>(tile1, Vec2{(float) m_worldConfig.tileWidth, (float) m_worldConfig.tileHeight}, true, true);
    world.set<CRect>(tile1, Vec2(m_worldConfig.tileWidth, m_worldConfig.tileHeight));
    auto& rect1 = world.get<CRect>(tile1).rect;
    rect1.setPosition(adjustedPos.x, adjustedPos.y);
    rect1.setFillColor(m_worldConfig.fc);
    rect1.setOutlineThickness(1);
    rect1.setOutlineColor(sf::Color::White);
}


void SceneBattle::spawnInteractable(const Vec2 &tilePos) {
    auto tile = world.createEntity("interactable");
    Vec2 adjustedPos{tilePos.x * (float) m_worldConfig.tileSize, tilePos.y * (float) m_worldConfig.tileSize};

    world.set<CTransform>(tile, adjustedPos, Vec2{0.f, 0.f}, 0.f);
    world.set<CBoundingBox>(tile, Vec2{(float) m_worldConfig.tileWidth, (float) m_worldConfig.tileHeight}, false, false);
    world.set<CRect>(tile, Vec2(m_worldConfig.tileWidth, m_worldConfig.tileHeight));
    world.set<CDialogue1>(tile, "../bin/config/dialogue.json", "npc1", 0, 0);

    auto& rect1 = world.get<CRect>(tile).rect;
    rect1.setPosition(adjustedPos.x, adjustedPos.y);
    rect1.setFillColor(sf::Color::Green);
    rect1.setOutlineThickness(1);
    rect1.setOutlineColor(sf::Color::White);
}

void SceneBattle::spawnEnemy(const Vec2& tilePos) {
    auto enemy = world.createEntity("enemy");
    Vec2 adjustedPos{tilePos.x * (float) m_worldConfig.tileSize, tilePos.y * (float) m_worldConfig.tileSize};
    Vec2 size{(float) m_worldConfig.tileWidth, (float) m_worldConfig.tileHeight};

    world.set<CTransform>(enemy, adjustedPos, Vec2{0.f, 0.f}, 0.f);
    world.set<CBoundingBox>(enemy, size, false, false);
    world.set<CAI>(enemy);
    world.set<CAISeekBehavior>(enemy);
    world.set<CMovable>(enemy);
    world.set<CRect>(enemy, Vec2(m_worldConfig.tileWidth, m_worldConfig.tileHeight));

    auto& rect1 = world.get<CRect>(enemy).rect;
    rect1.setPosition(adjustedPos.x, adjustedPos.y);
    rect1.setFillColor(sf::Color(219, 59, 104));
    rect1.setOutlineThickness(1);
    rect1.setOutlineColor(sf::Color::Cyan);
}

void SceneBattle::spawnFlyingEnemy(const Vec2& tilePos) {
}

Vec2 SceneBattle::calculateWorldPos() {
}

void SceneBattle::spawnHitbox(std::shared_ptr<Entity1> source, const Vec2 &pos, const Vec2& size, const Vec2& offset, float damage, bool active) {
}

void SceneBattle::spawnBullet(const Vec2& source, const Vec2& dist) {
}


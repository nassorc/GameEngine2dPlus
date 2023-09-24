
#include "SceneParallax.h"

SceneParallax::SceneParallax(GameEngine *gameEngine, Assets &assetManager, const std::string &config)
        : Scene(gameEngine, assetManager) {
    init(config);
}

void SceneParallax::init(const std::string& config) {
    m_assetManager.loadFromFile(config);
    Vec2 windowSize = m_game->getWindowSize();
    Vec2 intialSize(m_assetManager.getTexture("texNature1").getSize().x, m_assetManager.getTexture("texNature1").getSize().y);
    Vec2 texSize{intialSize.x, intialSize.y};

    Vec2 ratio{windowSize.x / texSize.x, windowSize.y / texSize.y};

    for(size_t idx=0; idx<10; idx++) {
        if(idx == 3 || idx == 8) { continue;}
        std::string tName = "texNature" + std::to_string(idx+1);
        sf::Sprite layer(m_assetManager.getTexture(tName), sf::IntRect(0, 0, windowSize.x * 10, windowSize.y));
        layer.scale(ratio.x, ratio.y);
        layers.push_back(layer);
    }

    // REGISTER INPUT ACTION ==========================================
    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::A, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");

    // ADD PLAYER =====================================================
    auto player = m_entityManager.addEntity("PLAYER");
    player->addComponent<CInput>();
    player->addComponent<CTransform>(Vec2(50, 50), Vec2(0, 0), 0);
    player->addComponent<CInput>();
    m_player = player;
}

void SceneParallax::update() {
    m_entityManager.update();
    sMovement();
    sRender();
}

void SceneParallax::sDoAction(const Action &action) {
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
    if (action.type() == "END") {
        if (action.name() == "UP") m_player->getComponent<CInput>().up = false;
        else if (action.name() == "DOWN") m_player->getComponent<CInput>().down = false;
        else if (action.name() == "LEFT") m_player->getComponent<CInput>().left = false;
        else if (action.name() == "RIGHT") m_player->getComponent<CInput>().right = false;
    }
}

void SceneParallax::sMovement() {
    // PLAYER MOVEMENT
    float SPEED{10};
    Vec2 velocity = {0.0f, 0.0f};
    auto &input = m_player->getComponent<CInput>();

    if (input.up) {
        velocity.y = SPEED * -1;
    } else if (input.down) {
        velocity.y = SPEED;
    } else if (input.left) {
        velocity.x = SPEED * -1;
    } else if (input.right) {
        velocity.x = SPEED;
    }

    m_player->getComponent<CTransform>().velocity = velocity;

    // update position
    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CTransform>()) {
            auto &transform = e->getComponent<CTransform>();
            transform.pos += transform.velocity;
        }
    }

}

void SceneParallax::sRender() {
    m_game->clearWindow(m_backgroundColor);

    // DRAW BACKGROUND ==================================================
    for(size_t idx=0; idx<layers.size(); idx++) {
        layers[idx].move(idx * -0.3, 0);
        m_game->window().draw(layers[idx]);
    }
    // ~DRAW BACKGROUND ==================================================

    // DRAW PLAYER =======================================================
    sf::CircleShape p(50);
    p.setFillColor(sf::Color(0, 0, 0));
    p.setOrigin(25,25);
    auto& pPos = m_player->getComponent<CTransform>().pos;
    p.setPosition(pPos.x, pPos.y);
    m_game->window().draw(p);
    // ~DRAW PLAYER ======================================================


    for (auto &e: m_entityManager.getEntities()) {
        if (e->hasComponent<CShape>()) {
            auto &shape = e->getComponent<CShape>().shape;
            m_game->window().draw(shape);
        }
    }
}

void SceneParallax::onEnd() {

}

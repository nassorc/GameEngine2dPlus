#include "SceneMenu.h"

#include "Scene.h"
#include "ScenePlay.h"
#include <map>
#include <deque>
#include <array>
#include "GameEngine.h"

SceneMenu::SceneMenu(GameEngine *gameEngine)
    : Scene(gameEngine)
{
    init();
}

void SceneMenu::init() {
    // register gameplay actions
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::D, "ENTER");
    registerAction(sf::Keyboard::Escape, "BACK");

    // setups : loading font sizes and data to display
    auto entity = m_entityManager.addEntity("TEXT");
    entity->addComponent<CText>(sf::Text("Zelda", m_game->getAssets().getDefaultFont(), 24));
}

void SceneMenu::update() {
    sRender();
}

void SceneMenu::onEnd() {

}

void SceneMenu::sDoAction(const Action &action) {
    if (action.type() == "START") {
        // if (action.name() == "PAUSE") { setPaused(!m_paused); }
        if (action.name() == "UP") {
            std::cout << "UP" << std::endl;
            // if (m_selectedMenuIndex > 0) m_selectedMenuIndex--;
            // else m_selectedMenuIndex = m_menuStrings.size() - 1;
        } else if (action.name() == "DOWN") {
            // if (m_selectedMenuIndex < 2) m_selectedMenuIndex++;
            // else m_selectedMenuIndex = 0;
        } else if (action.name() == "ENTER") {
            m_game->changeScene("PLAY", std::make_shared<ScenePlay>(m_game, "../bin/testlevel.txt"));
            // m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_menuLevelPaths[m_selectedMenuIndex]));
        } else if (action.name() == "BACK") {
            m_game->quit();
        }
    }
}

void SceneMenu::sRender() {
    m_entityManager.update();
    m_game->window().clear(sf::Color(111, 113, 232));

    for (auto& e : m_entityManager.getEntities()) {
        if(e->hasComponent<CAnimation>()) {
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            sprite.setScale(5, 5);
            m_game->window().draw(sprite);
        }
        if(e->hasComponent<CText>()) {
            m_game->window().draw(e->getComponent<CText>().text);
        }
    }
    // // RESET SELECTABLE ITEMS TO DEFAULT FILL COLOR
    // for (auto& e : m_fontEntities) {
    //   e.setFillColor(sf::Color(230,230,230));
    // }
    // // SET SELECTABLE COLOR
    // m_fontEntities.at(m_selectedMenuIndex).setFillColor(sf::Color(0, 0, 0));

    // // DRAW SELECTABLE ITEMS
    // m_game->window().draw(m_fontEntities.at(0));
    // m_game->window().draw(m_fontEntities.at(1));
    // m_game->window().draw(m_fontEntities.at(2));

     m_game->window().display();
}
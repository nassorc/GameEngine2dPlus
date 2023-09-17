#include "GameEngine.h"

#include "Scene.h"
#include "SceneMenu.h"
#include "ScenePlayground.h"
#include "SceneParallax.h"
#include "Assets.h"
#include "SFML/Graphics.hpp"
#include "Vec2.h"

#include <map>
#include <string>
#include <memory>
#include <iostream>


#include <imgui.h>
#include <imgui-SFML.h>

// <Texture specification> ===========================
// F: Texture N P
//		N                  Texture name         string
//		P                  Texture file path    string

// <Animation specification> =========================
// F: Animation N T F S
//		N                  Animation Name		string
//		T				   Texture Name         string
//		F				   Frame Count			int
//      S				   Animation Speed		int

GameEngine::GameEngine(const std::string &path) {
    init(path);
}

void GameEngine::init(const std::string &path) {
    // load asset
    if(!path.empty()) {
        m_assets.loadFromFile(path);
    }

    m_window.create(sf::VideoMode(1280, 768), "Zelda");
    m_window.setFramerateLimit(60);
    ImGui::SFML::Init(m_window);
    // First scene
//    changeScene("MENU", std::make_shared<ScenePlayground>(this));
//    changeScene("MENU", std::make_shared<SceneParallax>(this, "../bin/levelParallax.txt"));
    changeScene("MENU", std::make_shared<SceneMenu>(this));
}

void GameEngine::update() {
    sUserInput();
    currentScene()->update();
}

void GameEngine::sUserInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {

        if (event.type == sf::Event::Closed) {
            quit();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::X) {
                std::cout << "Screenshot saved\n";
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            // if the current scene does not have an action associated with this key, skip the event

            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) {
                continue;
            }

            // determine start or end action by whether it was key press or release
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // Because it exists, look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));

            // currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));

        }

        // mouse inputs are not mapped
        // mouse position is relative to the window
        // sf::Vector2f mpos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

        Vec2 mPos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).x);
         if (event.type == sf::Event::MouseMoved) {
         	currentScene()->doAction(Action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
         }

        // if (event.type == sf::Event::MouseButtonPressed) {
        // 	switch (event.mouseButton.button) {
        // 	case sf::Mouse::Left: { currentScene()->doAction(Action("LEFT_CLICK", "START", mpos)); break; }
        // 	case sf::Mouse::Middle: {break; }
        // 	case sf::Mouse::Right: {break; }
        // 	}
        // }
    }

//  m_window.clear(sf::Color(18, 218, 40));
//  // sf::Sprite sprite(m_assets.getTexture("TexCharacters"), sf::IntRect(0, 0, 16, 16));
//  // sprite.setScale(5, 5);
//  auto& s = m_assets.getAnimation("Stair");
//  s.getSprite().setScale(4, 4);
//  m_window.draw(s.getSprite());
//  m_window.display();

}

std::shared_ptr<Scene> GameEngine::currentScene() {
    return m_sceneMap[m_currentScene];
}

void GameEngine::changeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene) {
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;
}

void GameEngine::quit() {
    m_running = false;
}

void GameEngine::run() {
    while (isRunning()) {
        update();
    }
}

sf::RenderWindow &GameEngine::window() {
    return m_window;
}

bool GameEngine::isRunning() {
    return m_running;
}

Assets &GameEngine::getAssets() {
    return m_assets;
}
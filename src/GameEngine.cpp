#include "GameEngine.h"

#include "Scene.h"
#include "SceneMenu.h"
#include "ScenePlayground.h"
#include "SceneParallax.h"
#include "SceneBattle.h"
#include "Assets.h"
#include "SFML/Graphics.hpp"
#include "Vec2.h"
#include "NewArchitectureScene.h"

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


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
    // load configuration file
    m_fileReader.openFile(path);

    json config = json::parse(m_fileReader.readFileContents());
    std::cout << config << std::endl;
    // load settings
    json settings = json::parse(openFile(config[STARTUP_PREFERENCE_KEY]));

    json window = settings[WINDOW_KEY];
    json game   = settings[GAME_SETTINGS_KEY];

    unsigned int frameLimit   = game[FRAME_LIMIT_KEY];
    unsigned int windowWidth  = window[WINDOW_WIDTH_KEY];
    unsigned int windowHeight = window[WINDOW_HEIGHT_KEY];
    std::string windowName    = window[WINDOW_NAME_KEY];

    m_window.create(sf::VideoMode( windowWidth, windowHeight), windowName);
    m_window.setFramerateLimit(frameLimit);

    // load startup assets
//    m_assets.loadFromFile(config[STARTUP_ASSETS_KEY]);
    m_assets.loadFromFile(config[STARTUP_ASSETS_KEY]);
    ImGui::SFML::Init(m_window);
    // First scene
//    changeScene("MENU", std::make_shared<ScenePlayground>(this));
//    changeScene("MENU", std::make_shared<SceneParallax>(this, "../bin/levelParallax.txt"));
//    changeScene("MENU", std::make_shared<SceneMenu>(this, m_assets));
    changeScene("BATTLE", std::make_shared<SceneBattle>(this, m_assets));
//    changeScene("ARCH", std::make_shared<NewArchitectureScene>(this, m_assets));
}

void GameEngine::update() {
    sUserInput();
    currentScene()->update();
    // DISPLAY ENTITIES
    m_window.display();
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

        if (event.type == sf::Event::MouseMoved) {
            currentScene()->doAction(Action("MOUSE_MOVE", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
        }
    }
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

std::string GameEngine::openFile(const std::string &path) {
    std::ifstream fin(path);
    std::stringstream buffer;

    if(!fin.is_open()) {
        std::cout << "Cannot not open configuration file" << std::endl;
        exit(-1);
    }
    buffer << fin.rdbuf();
    return buffer.str();
}

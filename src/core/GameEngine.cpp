#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sstream>

#include "../scenes/SceneMenu.h"
#include "../scenes/SceneBattle.h"
#include "../utils/entityPrefabParser/EntityParser.h"
#include "Assets.h"
#include "CoreManager.h"
#include "GameEngine.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "nlohmann/json.hpp"
#include "SFML/Graphics.hpp"
#include "World.h"


using json = nlohmann::json;
World world;
std::shared_ptr<Entity1> g_selectedEntity;

GameEngine::GameEngine(const std::string &path) {
    init(path);
}
class ConfigParser {
    struct ConfigKey {
        
    };
public:
    ConfigParser(std::string config);
    void getValue(std::string key);
};


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
    m_coreManager = CoreManager();

    // set global variables
    world.window = &m_window;
    world.coreManager = new CoreManager();
    world.coreManager->init();

    // load startup assets
//    m_assets.loadFromFile(config[STARTUP_ASSETS_KEY]);
    m_assets.loadFromFile(config[STARTUP_ASSETS_KEY]);

    ImGui::SFML::Init(m_window);


    // First scene
    // TEST SCENES=====================================================================================
    // changeScene("MENU", std::make_shared<ScenePlayground>(this));
    // changeScene("MENU", std::make_shared<SceneParallax>(this, "../bin/levelParallax.txt"));
    // changeScene("MENU", std::make_shared<SceneMenu>(this, m_assets));
    changeScene("BATTLE", std::make_shared<SceneBattle>(this, m_assets));
    // TEST SCENES=====================================================================================
    // load entity prefabs
    std::ifstream fin("../bin/config/prefabs.json");
    json data = json::parse(fin);

    EntityParser entityParser;
    entityParser.parseFromJSON(data);
}

void GameEngine::update() {
    sUserInput();

    ImGui::SFML::Update(m_window, m_dt.restart());

    if(m_showLevelEditor) {
        ImGui::ShowDemoWindow();
        drawDevTools();
    }

    currentScene()->update();
    ImGui::SFML::Render(m_window);
    // DISPLAY ENTITIES
    m_window.display();
}

void GameEngine::sUserInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        // imgui process event
        ImGui::SFML::ProcessEvent(m_window, event);

        if (event.type == sf::Event::Closed) {
            quit();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::X) {
                std::cout << "Screenshot saved\n";
            }
            else if (event.key.code == sf::Keyboard::I) {
                m_showLevelEditor = !m_showLevelEditor;
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
        if(event.type == sf::Event::MouseButtonPressed) {
            currentScene()->doAction(Action("MOUSE_CLICK", "START", Vec2(event.mouseButton.x, event.mouseButton.y)));
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
    ImGui::SFML::Shutdown();
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

void GameEngine::drawDevTools() {
//    if(g_selectedEntity != nullptr) {
//        ImVec4 headerColor{0.9, 0.1, 0.1, 1};
//        ImGui::Begin("Entity");
//
//        ImGui::TextColored(headerColor, "Information");
//        ImGui::Text("tag:");
//        ImGui::SameLine();
//        ImGui::Text(g_selectedEntity->tag().c_str());
//        ImGui::Text("Id: ");
//        ImGui::SameLine();
//        ImGui::Text(std::to_string(g_selectedEntity->id()).c_str());
//
//
//        ImGui::TextColored(headerColor, "Components");
//        ImGui::SameLine();
//        ImGui::Button("Add component");
//
//
//        ImGui::End();
//
//        ImGui::Begin("Transform");
//        auto& pos = world.getComponent<CTransform>(g_selectedEntity).pos;
//        auto& velocity = world.getComponent<CTransform>(g_selectedEntity).velocity;
//        ImGui::InputFloat("X", &pos.x);
//        ImGui::InputFloat("Y", &pos.y);
//        ImGui::InputFloat("velocity x", &velocity.x);
//        ImGui::InputFloat("velocity y", &velocity.y);
//        ImGui::End();
//
//
//        ImGui::Begin("Gravity");
//        auto& gravity = world.getComponent<CGravity>(g_selectedEntity).gravity;
//        ImGui::InputFloat("gravity", &gravity);
//        ImGui::End();
//    }
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {350.f, 600.f});
//    ImGui::Begin("Devtools");



//    if(ImGui::BeginTable("entities", 3)) {
//        for(int row = 0; row < world.getEntities().size(); row++) {
//            ImGui::TableNextRow();
//            ImGui::TableSetColumnIndex(0);
////            ImGui::Text(world.getEntities()[row]->tag().c_str());
//            ImGui::PushID(row);
//            if(ImGui::Button(world.getEntities()[row]->tag().c_str())) {
//                ImGui::TableSetColumnIndex(1);
//                ImGui::Text(world.getEntities()[row]->tag().c_str());
//            }
//            ImGui::PopID();
//        }
//        ImGui::EndTable();
//    }



//    ImGui::End();
//    ImGui::PopStyleVar();
}

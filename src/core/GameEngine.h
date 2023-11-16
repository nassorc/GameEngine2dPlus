#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "Assets.h"
#include "CoreManager.h"
#include "../utils/levelParser/FileReader.h"
#include "../scenes/Scene.h"
#include "SFML/Graphics.hpp"

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

static const std::string STARTUP_PREFERENCE_KEY = "startup_preferences_file";
static const std::string STARTUP_ASSETS_KEY = "startup_assets_file";
static const std::string WINDOW_KEY = "window";
static const std::string WINDOW_NAME_KEY = "name";
static const std::string WINDOW_WIDTH_KEY = "width";
static const std::string WINDOW_HEIGHT_KEY = "height";
static const std::string GAME_SETTINGS_KEY = "game";
static const std::string FRAME_LIMIT_KEY = "frame_limit";

class GameEngine {
protected:
	sf::RenderWindow    m_window;
    CoreManager         m_coreManager;

	Assets              m_assets;
	std::string         m_currentScene;
	SceneMap            m_sceneMap;
	size_t              m_simulationSpeed = 1;
	bool                m_running = true;
    bool                m_showLevelEditor = false;
    sf::Clock           m_dt;
    FileReader          m_fileReader;

	void init(const std::string& path);
	void update();
	void sUserInput();
	std::shared_ptr<Scene> currentScene();

    std::string openFile(const std::string& path);

public:
	GameEngine(const std::string& path);
	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
	void quit();
	void run();
	sf::RenderWindow& window();
    void clearWindow(sf::Color color) {
        m_window.clear(color);
    }
    void draw(const sf::Sprite& e) {
        m_window.draw(e);
    }
    void draw(const sf::RectangleShape& e) {
        m_window.draw(e);
    }
    void draw(const sf::CircleShape& e) {
        m_window.draw(e);
    }
    void draw(const sf::Text e)  {
        m_window.draw(e);
    }
    Vec2 getWindowSize() {
        return Vec2{(float) m_window.getSize().x, (float) m_window.getSize().y};
    }
    void setView(const sf::View& view) {
        m_window.setView(view);
    }

    void drawDevTools();


	// const Assets& assets() const;
	bool isRunning();
	Assets& getAssets();
};
#pragma once

#include "Scene.h"
#include "ScenePlay.h"
#include <map>
#include <deque>
#include <array>

class SceneMenu : public Scene {
private:
  std::string m_title = "MEGA MARIO";
  std::string m_footer = "UP:W DOWN:S PLAY:D BACK:ESC";
protected:
	std::vector<std::string>     m_levelPaths;
	// std::shared_ptr<sf::Text>    m_menuText;
	// std::shared_ptr<sf::Text>    m_footerText;
	// sf::Text                   m_footerText;
  // sf::Font                   m_font;
	size_t                     m_selectedMenuIndex = 0;
	std::array<std::string, 3> m_menuStrings{"level 1", "level 2", "level 3"};
	std::array<std::string, 3> m_menuLevelPaths{ "bin/level1.txt", "testLevel.txt", "level3.txt" };
	// std::vector<sf::Text>      m_fontEntities;
	// FONT temp

	void init();
	void update(); 
	void onEnd();
	void sDoAction(const Action& action);

public:
	SceneMenu(GameEngine* gameEngine);
	void sRender();
};
#pragma once

#include "Scene.h"
#include "../(deprecated)/ScenePlay.h"
#include <map>
#include <deque>
#include <array>

class SceneMenu : public Scene {
private:
  std::string m_title = "MEGA MARIO";
  std::string m_footer = "UP:W DOWN:S PLAY:D BACK:ESC";
protected:
	std::vector<std::string>     m_levelPaths;
	void init();
	void update(); 
	void onEnd();
	void sDoAction(const Action& action);

public:
	SceneMenu(GameEngine *gameEngine, Assets &assetManager);
	void sRender();
};
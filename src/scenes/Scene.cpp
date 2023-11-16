#include "Scene.h"

#include "../core/Action.h"
// #include "EntityManager.h"
#include <iostream>
#include <map>

Scene::Scene(GameEngine *gameEngine, Assets &assetManager)
    : m_game(gameEngine)
    , m_assetManager(assetManager) {}

void Scene::setPaused(bool paused) {
  m_paused = paused;
}

void Scene::doAction(const Action& action) {
  sDoAction(action);
}

void Scene::registerAction(int inputKey, const std::string& actionName) {
  m_actionMap[inputKey] = actionName;
}

const ActionMap& Scene::getActionMap() const {
  return m_actionMap;
}
#include "Scene.h"

#include "Action.h"
// #include "EntityManager.h"
#include <iostream>
#include <map>

Scene::Scene(GameEngine* gameEngine) {
  m_game = gameEngine;
}

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
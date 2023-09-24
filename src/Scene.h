#pragma once

#include <iostream>
#include <map>
#include "Action.h"
#include "EntityManager.h"
#include "Assets.h"

typedef std::map<int, std::string> ActionMap;

class GameEngine;

/*
* The scene layer is on top of entity manager, each scene has its own entities to manage
* store common scene data: entities, frame count, actions
*/
class Scene {
protected:
    GameEngine*       m_game = nullptr;
    Assets&           m_assetManager;
    EntityManager     m_entityManager;
    ActionMap         m_actionMap;
    size_t            m_currentFrame = 0;

    bool              m_paused = false;
    bool              m_hasEnded = false;

    virtual void onEnd() = 0;
    void setPaused(bool paused);
    bool hasEnded() const;

public:
    Scene(GameEngine *gameEngine, Assets &assetManager);
    // function simulate calls the derived scene's update() a given number of times
    void simulate(const size_t frames);
    void registerAction(int inputKey, const std::string &actionName);
    const ActionMap &getActionMap() const;
    void doAction(const Action &action);
    virtual void update() = 0;
    virtual void sDoAction(const Action &action) = 0;
    virtual void sRender() = 0;
};
#pragma once

#include "Action.h"
#include "EntityManager.h"
#include <iostream>
#include <map>

typedef std::map<int, std::string> ActionMap;

class GameEngine;

/*
* The scene layer is on top of entity manager, each scene has its own entities to manage
* store common scene data: entities, frame count, actions
*/
class Scene {
protected:
    EntityManager m_entityManager;
    GameEngine *m_game = nullptr;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

    virtual void onEnd() = 0;

public:
    Scene(GameEngine *gameEngine);

    void setPaused(bool paused);

    bool hasEnded() const;

    // function simulate calls the derived scene's update() a given number of times
    void simulate(const size_t frames);

    void registerAction(int inputKey, const std::string &actionName);

    const ActionMap &getActionMap() const;
    // void drawLine(const sf::Vector2f p1, const sf::Vector2f& p2);

    void doAction(const Action &action);

    virtual void update() = 0;

    virtual void sDoAction(const Action &action) = 0;

    virtual void sRender() = 0;
};
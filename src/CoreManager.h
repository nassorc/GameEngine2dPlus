//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <memory>
#include "EntityManager1.h"
#include "ComponentManager1.h"
#include "SystemManager1.h"

class Test{
    int x;
};

class CoreManager {
private:
    std::shared_ptr<SystemManager1> m_systemManager;
    std::shared_ptr<EntityManager1> m_entityManager;
    std::shared_ptr<ComponentManager1> m_componentManager;

public:
    CoreManager() {
    }
    void init() {
        m_systemManager = std::make_shared<SystemManager1>();
        m_componentManager= std::make_shared<ComponentManager1>();
        m_entityManager = std::make_shared<EntityManager1>(m_componentManager);
    }
    shared_ptr<Entity1> createEntity(std::string tag = "default") {
        return m_entityManager->createEntity(tag);
    }

    void destroyEntity();

    template<class T>
    void registerComponent() {
        m_componentManager->registerComponent<T>();
    }

    template<class T>
    void addComponent(std::shared_ptr<Entity1> e, T component) {
        m_componentManager->addComponent(e, component);
        Signature entitySignature = e->getSignature();
        entitySignature.set(m_componentManager->getComponentType<T>(), true);
        e->setSignature(entitySignature);

        m_systemManager->entitySignatureChange(e, entitySignature);
    }
    template<class T>
    T& getComponent(std::shared_ptr<Entity1> e) {
        return m_componentManager->getComponent<T>(e);
    }

    template<class T>
    size_t getComponentType() {
        return m_componentManager->getComponentType<T>();
    }

    template<class T>
    void registerSystem() {
        m_systemManager->registerSystem<T>();
    }

    template<class T>
    void setSystemSignature(Signature signature) {
        m_systemManager->setSignature<T>(signature);
    }

    void updateSystems() {
        m_systemManager->updateSystems();
    }
};

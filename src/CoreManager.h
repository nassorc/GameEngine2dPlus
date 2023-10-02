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
        m_entityManager = std::make_shared<EntityManager1>();
    }
    shared_ptr<Entity1> createEntity(std::string tag = "default") {
        return m_entityManager->createEntity(tag);
    }

    void update() {
        // add the new entities to the system manager before invoking
        // the entity manager's update function. Creating an entity
        // through the core manager's createEntity function, puts the
        // new entity in the m_entityManager's toAddEntity queue, and it
        // is added to the list of entities when we call its update function,
        // which will be on the next frame, and it will clear to queue.
        // The system manager must receive these entities before updating
        // the entity manager.
        // This structure of queuing the entities and adding them at the start
        // of the new frame instead of real time adding and removing entities
        // prevents iterator invalidation.

        for (auto& e : m_entityManager->getNewEntities()) {
            m_systemManager->entitySignatureChange(e, e->getSignature());
        }
        m_entityManager->update();
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

//        m_systemManager->entitySignatureChange(e, entitySignature);
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

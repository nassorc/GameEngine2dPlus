//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

#include <memory>
#include <queue>
#include "EntityManager1.h"
#include "ComponentManager1.h"
#include "SystemManager1.h"
#include "Types.h"
#include "../components/Components.h"


enum class PrefabType {
    FIREBALL
};

class CoreManager {
private:
    std::vector<std::shared_ptr<Entity1>> m_entitiesToRemove;
    std::map<PrefabType, std::function<std::shared_ptr<Entity1>(void)>> prefabEntityMap;

public:
    std::shared_ptr<SystemManager1>    m_systemManager;
    std::shared_ptr<EntityManager1>    m_entityManager;
    std::shared_ptr<ComponentManager1> m_componentManager;

    CoreManager() {
    }
    void init() {
        m_systemManager = std::make_shared<SystemManager1>();
        m_componentManager= std::make_shared<ComponentManager1>();
        m_entityManager = std::make_shared<EntityManager1>();
    }

    void update() {
        /*
        * add the new entities to the system manager before invoking
        * the entity manager's update function. Creating an entity
        * through the core manager's createEntity function, puts the
        * new entity in the m_entityManager's toAddEntity queue, and it
        * is added to the list of entities when we call its update function,
        * which will be on the next frame, and it will clear to queue.
        * The system manager must receive these entities before updating
        * the entity manager.
        * This structure of queuing the entities and adding them at the start
        * of the new frame instead of real time adding and removing entities
        * prevents iterator invalidation.
        */
        m_entityManager->update();
        for(auto entity : m_entitiesToRemove) {
            m_componentManager->entityDestroyed(entity);
            // remove from system
            m_systemManager->entityDestroyed(entity);
            // remove from entity manager
            m_entityManager->removeEntity(entity);
        }
        m_entitiesToRemove.clear();
    }

    shared_ptr<Entity1> createEntity(std::string tag = "default") {
        return m_entityManager->createEntity(tag);
    }
    void removeEntity(std::shared_ptr<Entity1> entity) {
        m_entitiesToRemove.push_back(entity);
    }

    template<class T>
    std::vector<std::shared_ptr<Entity1>> queryEntitiesByType() {
        // get component's bitset position
        size_t bitIdx = m_componentManager->getComponentType<T>();
        return m_entityManager->getEntities(bitIdx);
        // find entities with their bitset signature set to true at index bitIdx

    }

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
    void addComponentByTypeName(std::string typeName, std::shared_ptr<Entity1> e, T* component) {
//        auto r = static_cast<CRect* >(component)->rect;
//        std::cout << "In component: " << (int) r.getFillColor().r << std::endl;
        m_componentManager->addComponentByTypeName(typeName, e, component);
        Signature entitySignature = e->getSignature();
        entitySignature.set(m_componentManager->getComponentTypeByTypeName(typeName), true);
        e->setSignature(entitySignature);

        m_systemManager->entitySignatureChange(e, entitySignature);
    }
    template<class T>
    void removeComponent(std::shared_ptr<Entity1> e) {
        m_componentManager->removeComponentFromEntity<T>(e);
        Signature entitySignature = e->getSignature();
        e->setSignature(entitySignature.set(m_componentManager->getComponentType<T>(), false));
        m_systemManager->entitySignatureChange(e, entitySignature);
    }
    template<class T>
    bool hasComponent(std::shared_ptr<Entity1> e) {
        Signature eSignature = e->getSignature();
        return eSignature.test(m_componentManager->getComponentType<T>());
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
    CoreManager* registerSystem() {
        m_systemManager->registerSystem<T>();
        return this;
    }
    template<class T>
    std::shared_ptr<T> getSystem() {
//        m_systemManager->registerSystem<T>();
        return m_systemManager->getSystem<T>();
    }
    template<class T>
    CoreManager* setSystemSignature() {
        Signature signature;
        signature.set(getComponentType<T>());
        m_systemManager->setSignature<T>(signature);
        return this;
    }

    template<class T>
    void setSystemSignature(Signature signature) {
        m_systemManager->setSignature<T>(signature);
    }

    void updateSystems() {
        m_systemManager->updateSystems();
    }

    Entity1Vector getEntities() {
        return m_entityManager->getEntities();
    }

    std::shared_ptr<Entity1> prefab(PrefabType type) {
        if(type == PrefabType::FIREBALL) {
            return spawnFireball();
        }
        return spawnFireball();
    }

    std::shared_ptr<Entity1> spawnFireball() {
        std::cout << "spawned fireball" << std::endl;
        auto bullet = m_entityManager->createEntity("fireball");

        // adding components inside of the coreManager class must be done through its own method instead of directly
        // adding one through the component manager.
        this->addComponent(bullet, CTransform{Vec2{200.f, 200.f}, Vec2{4.f, 0.f}, 0});
        this->addComponent(bullet, CBoundingBox{Vec2{30.f, 30.f}, Vec2{0.f, 0.f}, false, false});
        this->addComponent(bullet, CRect{Vec2{30, 30}});
        this->addComponent(bullet, CLifespan{30});

        auto& rect = m_componentManager->getComponent<CRect>(bullet).rect;
        rect.setPosition(200, 200);
        rect.setSize(sf::Vector2f(15, 15));
        rect.setFillColor(sf::Color::Yellow);
        rect.setOutlineThickness(2);
        rect.setOutlineColor(sf::Color::Yellow);
        return bullet;
    }
};

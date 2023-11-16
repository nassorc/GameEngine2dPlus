//
// Created by Matthew Crossan on 10/8/23.
//

#ifndef LIGHTCASTING_WORLD_H
#define LIGHTCASTING_WORLD_H

#include "CoreManager.h"
#include "Entity1.h"

struct World {
    CoreManager* coreManager = nullptr;
    sf::RenderWindow* window = nullptr;

    World() {}

    shared_ptr<Entity1> createEntity(std::string tag) {
        return coreManager->createEntity(tag);
    }

    template<class T, class... Args>
    void set(std::shared_ptr<Entity1> e, Args... args) {
        coreManager->addComponent(e, T{args...});
    }

    template<class T>
    T& get(std::shared_ptr<Entity1> e) {
        return coreManager->getComponent<T>(e);
    }

    Entity1Vector getEntities() {
        return coreManager->getEntities();
    }

    template<class T>
    bool has(std::shared_ptr<Entity1> e) {
        return coreManager->hasComponent<T>(e);
    }

    template<class T>
    std::vector<std::shared_ptr<Entity1>> query() {
        return coreManager->queryEntitiesByType<T>();
    }

    template<class T>
    void registerComponent() {
        coreManager->registerComponent<T>();
    }

    template<class T>
    size_t getComponentType() {
        return coreManager->getComponentType<T>();
    }

    template<class T>
    void registerSystem() {
        coreManager->registerSystem<T>();
    }

    template<class T>
    void setSystemSignature(Signature signature) {
        coreManager->setSystemSignature<T>(signature);
    }
};


#endif //LIGHTCASTING_WORLD_H

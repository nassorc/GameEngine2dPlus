//
// Created by Matthew Crossan on 10/1/23.
//

#pragma once

// class ComponentManager;

// array of components<Position> [P1, P3, ]
// E->I:
// [10:0] Entity 10 data is stored at index 0
// [16,1] Entity 16 data is store at index 2
//
// I->E:
// [0:10] At index 0, we have Entity 10's data
// [1:16]
//
// DELETION: we replace the deleted entities data with the last element
#include <iostream>
#include "../components/Components.h"
#include "Entity1.h"

using namespace std;
using ComponentType = std::size_t;

class IComponentArray {
protected:
    size_t size = 0;
    static const size_t capacity = 5000;

public:
    IComponentArray() = default;
    virtual void entityDestroyed(std::shared_ptr<Entity1> entity) = 0;

    template<class T>
    void InsertData(size_t entityId, T component) {};
    size_t getSize() { return size; }
};

template <class T> class ComponentArray : public IComponentArray {
    using EntityId = size_t;

    array<T, capacity> m_componentArray;                  // component data of the entities
    unordered_map<EntityId, size_t> m_entityToIndexMap;  // mapping of the entity's id to its data (index) in the array
    unordered_map<size_t, EntityId> m_indexToEntityMap;  // maps the data in a given position to an entity's id

public:
    ComponentArray() {}
    size_t getSize() { return size; }

    void InsertData(size_t entityId, T component) {
        size_t newIndex = size;
        // EntityId entityId = entity->getId();

        m_entityToIndexMap[entityId] = newIndex;
        m_indexToEntityMap[newIndex] = entityId;
        m_componentArray[newIndex] = component;

        size++;
    }
    /*
     * This function removes the component data of an Entity by
     * replacing the affected component with the last element in
     * the array. This keeps the array packed with no holes.
     */
    void removeData(shared_ptr<Entity1> entity) {
        // get entity's component index
        EntityId entityId = entity->id();
        size_t deletedEntityIdx = m_entityToIndexMap[entityId];  // index of component to delete
        size_t lastIdx = size - 1;
        size_t lastEntityIdx = m_entityToIndexMap[lastIdx];
        // replaces deleted entities data with the last one on the list
        m_componentArray[deletedEntityIdx] = m_componentArray[lastEntityIdx];

        // update map to point at correct entity
        EntityId entityOfLastElement = m_indexToEntityMap[lastEntityIdx];
        m_entityToIndexMap[entityOfLastElement] = deletedEntityIdx;
        m_indexToEntityMap[deletedEntityIdx] = entityOfLastElement;

        m_entityToIndexMap.erase(entityId);
        m_indexToEntityMap.erase(lastEntityIdx);

        size--;
    }
    T &getData(size_t entityId) {
        return m_componentArray[m_entityToIndexMap[entityId]];
    }
    void entityDestroyed(std::shared_ptr<Entity1> entity) {
        if(m_entityToIndexMap.find(entity->id()) != m_entityToIndexMap.end()) {
            removeData(entity);
        }

    }
};

// has the component array
// can add a component for an entity
class ComponentManager1 {
    // componentType records the components from 0 to n
    // the number represents what bit type the component represents
    // Setting "CTransform" component with a ComponentType(size_t) of 1 on entity = 0010
private:
    size_t m_nextComponentType = 0;                                // represents the bit position in a signature
    unordered_map<string, shared_ptr<IComponentArray>> m_componentMap;  // maps components of the same type to a ComponentArray
    unordered_map<string, size_t> m_componentTypeMap;            // maps components of the same type to a Bit position (used for creating signatures)

public:
    ComponentManager1() {}

    /*
    * Registers the component.
    * Assigns the component a bit position, given the m_nextComponentType (ComponentType).
    * Inserts a new instance of the component in a map.
    */
    template <class T> void registerComponent() {
        string typeName = typeid(T).name();                      // will be used to unique identify a unique component
//        std::cout << "registering component " << typeName << " " << m_nextComponentType << std::endl;
        auto component = std::make_shared<ComponentArray<T>>();  // creates an instance of a component array for the given component

        m_componentMap.insert({typeName, component});
        m_componentTypeMap.insert({typeName, (size_t) m_nextComponentType});

        m_nextComponentType++;
    }

    template<class T>
    bool addComponentByTypeName(std::string typeName, shared_ptr<Entity1> entity, T component) {
//        auto r = static_cast<CRect* >(component)->rect;
//        std::cout << "In component MM: " << (int) r.getFillColor().r << std::endl;
        if(m_componentMap.find(typeName) == m_componentMap.end()) { return false; }
        std::cout << "\nSIZESIZE " << m_componentMap[typeName]->getSize() << std::endl;
        m_componentMap[typeName]->InsertData(entity->id(), *component);
        std::cout << "\nSIZESIZE " << m_componentMap[typeName]->getSize() << std::endl;
        return true;
    }

    /*
    * Adds component to an entity
    */
    template <class T>
    void addComponent(shared_ptr<Entity1> entity, T component) {
        string typeName = typeid(T).name();
        auto arr = std::static_pointer_cast<ComponentArray<T>>(m_componentMap[typeName]);
        arr->InsertData(entity->id(), component);
    }

    template <class T>
    size_t getComponentType() {
        string typeName = typeid(T).name();
        return m_componentTypeMap[typeName];
    }
    size_t getComponentTypeByTypeName(std::string typeName) {
        return m_componentTypeMap[typeName];
    }

    template <class T>
    T &getComponent(shared_ptr<Entity1> entity) {
        string typeName = typeid(T).name();
        // get component array
        auto arr = std::static_pointer_cast<ComponentArray<T>>(m_componentMap[typeName]);

        return arr->getData(entity->id());
    }

    template <class T>
    void removeComponentFromEntity(shared_ptr<Entity1> entity) {
        string typeName = typeid(T).name();
        auto arr = std::static_pointer_cast<ComponentArray<T>>(m_componentMap[typeName]);
        arr->removeData(entity);
    }
    void entityDestroyed(shared_ptr<Entity1> entity) {
        std::cout << "destroying component entity" << std::endl;
        for(auto& [typeName, componentArr] : m_componentMap) {
            componentArr->entityDestroyed(entity);
        }
    }

};
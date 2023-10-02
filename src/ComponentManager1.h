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
#include "Entity1.h"
#include "Components1.h"
// #include "EntityManager.h"

using namespace std;
using ComponentType = std::size_t;

class IComponentArray {
public:
    IComponentArray() = default;
};

template <class T> class ComponentArray : public IComponentArray {
    using EntityId = size_t;
private:
    size_t size = 0;
    static const size_t capcity = 5000;

    array<T, capcity> m_componentArray;
    unordered_map<EntityId, size_t> m_entityToIndexMap;
    unordered_map<size_t, EntityId> m_indexToEntityMap;

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
    void removeData(shared_ptr<Entity1> entity) {
        // get entity's component index
        EntityId entityId = entity->getId();
        size_t deletedEntityIdx = m_entityToIndexMap[entityId];
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

    /*
    * Adds component to an entity
    */
    template <class T>
    void addComponent(shared_ptr<Entity1> entity, T component) {
        string typeName = typeid(T).name();
        auto arr =
                std::static_pointer_cast<ComponentArray<T>>(m_componentMap[typeName]);
        arr->InsertData(entity->getId(), component);
    }

    template <class T>
    size_t getComponentType() {
        string typeName = typeid(T).name();
        return m_componentTypeMap[typeName];
    }

    template <class T> T &getComponent(shared_ptr<Entity1> entity) {
        string typeName = typeid(T).name();
        // get component array
        auto arr = std::static_pointer_cast<ComponentArray<T>>(m_componentMap[typeName]);

        return arr->getData(entity->getId());
    }
};
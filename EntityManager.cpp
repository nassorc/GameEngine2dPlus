#include "EntityManager.h"

#include "Entity.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <map>
#include <variant>


void EntityManager::removeDeadEntities(EntityVector& vec) 
{
  auto itr = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity>& e) {
    return !e->isActive();
  });

  vec.erase(itr, vec.end());
}

EntityManager::EntityManager() 
{
}

void EntityManager::init() 
{
}

void EntityManager::update() 
{
  for (auto& e : m_entitiesToAdd)
  {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  removeDeadEntities(m_entities);

  for (auto& em : m_entityMap) 
  {
    removeDeadEntities(em.second);
  }
  m_entitiesToAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(std::string tag) 
{
  // only way of adding an entity  // delayed
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
  
  m_entitiesToAdd.push_back(entity);
  return entity;
}

EntityVector EntityManager::getEntities() 
{
  return m_entities;
}

EntityVector EntityManager::getEntities(std::string tag) 
{
  return m_entityMap[tag];
}
#include "./EntityManager.h"
#include <iostream>
#include "Components/ColliderComponent.h"
#include "./Collision.h"
#include <SDL2/SDL.h>

EntityManager::EntityManager() {}

void EntityManager::clearData() {
    for (auto& entity: m_entities) {
        entity->destroy();
    }
}
void EntityManager::update(float deltaTime) {
    for (auto& entity: m_entities) {
        entity->update(deltaTime);
    }

    destroyInactiveEntities();
}

void EntityManager::destroyInactiveEntities() {
    for (int i = 0; i < m_entities.size(); i++) {
        if (!m_entities[i]->isActive()) {
            m_entities.erase(m_entities.begin() + i);
        }
    }
}

void EntityManager::render() {
    // Render all entities by layer
    for (int layerNum = 0; layerNum < NUM_LAYERS; layerNum++) {
        for(auto& entity: getEntitiesByLayer(static_cast<LayerType>(layerNum))) {
            entity->render();
        }
    }
}

bool EntityManager::hasNoEntities() {
    return m_entities.size() == 0;
}

Entity& EntityManager::addEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    m_entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::getEntities() const {
    return m_entities;
}

std::vector<Entity*> EntityManager::getEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for(auto& entity: m_entities) {
        if(entity->getLayer() == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

unsigned int EntityManager::getEntityCount() {
    return m_entities.size();
}

void EntityManager::listAllEntities() const {
    for(auto& entity: m_entities) {
        std::cout << "Entity Name: " << entity->getName() << std::endl;
        entity->listAllComponents();
    }
}

void EntityManager::showCollisionBounds() {
    for (auto& entity: m_entities) {
        if (entity->hasComponent<ColliderComponent>()) {
            ColliderComponent* collider = entity->getComponent<ColliderComponent>();
            collider->setShowBounds(true);
        }
    }
}

Entity* EntityManager::getEntityByName(std::string entityName) {
    for (auto& entity: m_entities) {
        if (entity->getName().compare(entityName) == 0) {
            return entity;
        }
    }
}

CollisionType EntityManager::checkCollisions() const {
    for (auto& thisEntity : m_entities) {
        if (thisEntity->hasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->getComponent<ColliderComponent>();

            for (auto& otherEntity : m_entities) {
                if (thisEntity->getName().compare(otherEntity->getName()) != 0 && otherEntity->hasComponent<ColliderComponent>()) {
                    ColliderComponent* otherCollider = otherEntity->getComponent<ColliderComponent>();

                    if (Collision::checkRectangleCollision(thisCollider->getCollider(), otherCollider->getCollider())) {
                        if (thisCollider->getColliderTag().compare("PLAYER") == 0 && otherCollider->getColliderTag().compare("ENEMY") == 0)
                            return PLAYER_ENEMY_COLLISION;
                        if (thisCollider->getColliderTag().compare("PLAYER") == 0 && otherCollider->getColliderTag().compare("PROJECTILE") == 0)
                            return PLAYER_PROJECTILE_COLLISION;
                        if (thisCollider->getColliderTag().compare("ENEMY") == 0 && otherCollider->getColliderTag().compare("FRIENDLY_PROJECTILE") == 0)
                            return ENEMY_PROJECTLE_COLLISION;
                        if (thisCollider->getColliderTag().compare("PLAYER") == 0 && otherCollider->getColliderTag().compare("LEVEL_COMPLETE") == 0)
                            return PLAYER_LEVEL_COMPLETE_COLLISION;                         
                    }
                }
            }
        }
    }

    return NO_COLLISION;
}
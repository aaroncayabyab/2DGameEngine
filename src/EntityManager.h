#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "./Entity.h"
#include "./Constants.h"
#include <string>
#include <vector>

class Entity;

class EntityManager {
    private:
        std::vector<Entity*> m_entities;
    public:
        EntityManager();
        void clearData();
        void update(float deltaTime);
        void render();
        bool hasNoEntities();
        CollisionType checkCollisions() const;
        void showCollisionBounds();
        Entity& addEntity(std::string entityName, LayerType layer);
        std::vector<Entity*> getEntities() const;
        std::vector<Entity*> getEntitiesByLayer(LayerType layer) const;
        void listAllEntities() const;
        unsigned int getEntityCount();
        void destroyInactiveEntities();
        Entity* getEntityByName(std::string name);
};

#endif
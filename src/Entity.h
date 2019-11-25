#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <map>
#include <string>
#include "./EntityManager.h"
#include "./Component.h"
#include "./Constants.h"

class EntityManager;

class Entity {
    private:
        EntityManager& m_manager;
        bool m_isActive;
        std::string m_name;
        std::vector<Component*> m_components;
        std::map<const std::type_info*, Component*> m_componentTypeMap;
        LayerType m_layer;

    public:
        Entity(EntityManager& manager);
        Entity(EntityManager& manager, std::string name, LayerType layer);

        void update(float deltaTime);
        void render();
        void destroy();
        bool isActive() const;
        void listAllComponents() const;
        std::string getName() const;
        LayerType getLayer() const;

        template <typename T, typename ...TArgs>
        T& addComponent(TArgs&&... args) {
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->setOwner(this);
            m_components.emplace_back(newComponent);
            m_componentTypeMap[&typeid(*newComponent)] = newComponent;
            newComponent->initialize();
            return *newComponent;
        }

        template <typename T>
        T* getComponent() {
            return static_cast<T*>(m_componentTypeMap[&typeid(T)]);
        }

        template <typename T>
        bool hasComponent() const {
            return m_componentTypeMap.find(&typeid(T)) != m_componentTypeMap.end() ? true: false;
        }
};

#endif
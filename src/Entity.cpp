#include "./Entity.h"
#include <iostream>

Entity::Entity(EntityManager& manager): m_manager(manager) {
    m_isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, LayerType layer): m_manager(manager),  m_name(name), m_layer(layer) {
    m_isActive = true;
}

void Entity::update(float deltaTime) {
    for (auto& component: m_components) {
        component->update(deltaTime);
    }
}

void Entity::render() {
    for (auto& component: m_components) {
        component->render();
    }
}

void Entity::destroy() {
    m_isActive = false;
}

bool Entity::isActive() const {
    return m_isActive;
}

std::string Entity::getName() const {
    return m_name;
}

LayerType Entity::getLayer() const {
    return m_layer;
}

void Entity::listAllComponents() const {
    for (auto& componentType: m_componentTypeMap) {
        std::cout << "Component <" << componentType.first->name() << ">" << std::endl;
    }
}
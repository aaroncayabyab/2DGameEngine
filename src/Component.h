#ifndef COMPONENT_H
#define COMPONENT_H

#include "./Entity.h"

class Entity;

class Component {
    protected:
        Entity* m_owner;
    
    public:
        ~Component() {};
        virtual void initialize() {};
        virtual void update(float deltaTime) {};
        virtual void render() {};

        void setOwner(Entity* owner) {
            m_owner = owner;
        }
};

#endif
#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"
#include "../Game.h"
#include "../Constants.h"

class TransformComponent: public Component {
    private:
        glm::vec2 m_position;
        glm::vec2 m_velocity;
        int m_width;
        int m_height;
        int m_scale;
    
    public:
        // Transform Component for 2D Entities
        TransformComponent(int posX, int posY, int velX, int velY, int width, int height, int scale) {
            m_position = glm::vec2(posX, posY);
            m_velocity = glm::vec2(velX, velY);
            m_width = width;
            m_height = height;
            m_scale = scale;
        }

        void initialize() override {
        
        }

        void update(float deltaTime) override {
            m_position.x += m_velocity.x * deltaTime;
            m_position.y += m_velocity.y * deltaTime;
        }

        int getWidth() {
            return m_width;
        }

        int getHeight() {
            return m_height;
        }

        void setVelocity(glm::vec2 velocity) {
            m_velocity = velocity;
        }

        glm::vec2 getVelocity() {
            return m_velocity;
        }

        void setScale(int scale) {
            m_scale = scale;
        }

        int getScale() {
            return m_scale;
        }

        void setPosition(glm::vec2 position) {
            m_position = position;
        }

        glm::vec2 getPosition() {
            return m_position;
        }

        void render() override {
        }
};

#endif
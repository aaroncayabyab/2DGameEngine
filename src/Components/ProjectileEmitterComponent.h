#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"

class ProjectileEmitterComponent: public Component {
    private:
        TransformComponent* m_transform;
        glm::vec2 m_origin;
        int m_speed;
        int m_range;
        float m_angleRad;
        bool m_shouldLoop;
    public:
        ProjectileEmitterComponent(int speed, int angleDeg, int range, bool shouldLoop) {
            m_speed = speed;
            m_range = range;
            m_shouldLoop = shouldLoop;
            m_angleRad = glm::radians(static_cast<float>(angleDeg));
        }

        void initialize() override {
            m_transform = m_owner->getComponent<TransformComponent>();
            m_origin = { m_transform->getPosition().x, m_transform->getPosition().y };
            m_transform->setVelocity({ glm::cos(m_angleRad) * m_speed, glm::sin(m_angleRad) * m_speed });

        }

        void update(float deltaTime) override {
            if (glm::distance(m_transform->getPosition(), m_origin) > m_range) {
                if (m_shouldLoop) {
                    m_transform->setPosition({ m_origin.x, m_origin.y });
                }
                else {
                    m_owner->destroy();
                }
            }
        }

        void render() override {

        }
};

#endif
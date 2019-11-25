#ifndef COLLIDERCOMPONENT_H
#define COLLIDEROCMPONENT_H

#include <SDL2/SDL.h>
#include "../Collision.h"
#include "../EntityManager.h"
#include "../Game.h"
#include "./TransformComponent.h"

class ColliderComponent : public Component {
    private:
        std::string m_colliderTag;
        SDL_Rect m_collider;
        SDL_Rect m_src;
        SDL_Rect m_dest;

        bool m_showBounds;
        SDL_Texture* m_boundsTexture;

        TransformComponent* m_transform;
    public:
        ColliderComponent(std::string colliderTag, int x, int y, int w, int h) {
            m_colliderTag = colliderTag;
            m_collider = {x, y, w, h};

            m_showBounds = false;
        }

        ColliderComponent(std::string colliderTag, int x, int y, int w, int h, bool showBounds) {
            m_colliderTag = colliderTag;
            m_collider = {x, y, w, h};

            m_showBounds = showBounds;
        }

        void setShowBounds(bool showBounds) {
            m_showBounds = showBounds;
        }

        void initialize() override {
            if (m_owner->hasComponent<TransformComponent>()) {
                m_transform = m_owner->getComponent<TransformComponent>();
                m_src = {0, 0, m_transform->getWidth(), m_transform->getWidth()};
                m_dest = m_collider;
            }
        }

        void update(float deltaTime) override {
            m_collider.x = static_cast<int>(m_transform->getPosition().x);
            m_collider.y = static_cast<int>(m_transform->getPosition().y);
            m_collider.w = m_transform->getWidth() * m_transform->getScale();
            m_collider.h = m_transform->getHeight() * m_transform->getScale();

            m_dest.x = m_collider.x - Game::camera.x;
            m_dest.y = m_collider.y - Game::camera.y;
        }

        void render() override {
            if (m_showBounds == true) {
                SDL_Rect boundingBox = m_dest;
                SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(Game::renderer, &boundingBox);
            }
        }

        SDL_Rect getCollider() {
            return m_collider;
        }

        std::string getColliderTag() {
            return m_colliderTag;
        }

};


#endif
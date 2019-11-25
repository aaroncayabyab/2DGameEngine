#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"

using namespace std;

struct Animation {
    int index;
    int numFrames;
    int animationSpeed;
};

class SpriteComponent: public Component {
    private:
        // Sprite variables
        TransformComponent* m_transform;
        SDL_Texture* m_texture;
        SDL_Rect m_src;
        SDL_Rect m_dest;
        SDL_RendererFlip m_spriteFlip = SDL_FLIP_NONE;

        // Animation variables
        bool m_isFixed;
        bool m_isAnimated;
        int m_numFrames;
        int m_animationSpeed;
        unsigned int m_animationIndex = 0;
        std::string m_currentAnimation;

        std::map<std::string, Animation> m_animations;

    public:
        // Static Sprite
        SpriteComponent(std::string filePath) {
            setTexture(filePath);
            m_isAnimated = false;
            m_isFixed = false;
        }
        SpriteComponent(std::string filePath, bool isFixed) {
            setTexture(filePath);
            m_isAnimated = false;
            m_isFixed = isFixed;
        }

        // Animated Sprite
        SpriteComponent(std::string filePath, int numFrames, int animationSpeed, bool hasDirections, bool isFixed) {
            m_isAnimated = true;
            m_isFixed = isFixed;
            m_numFrames = numFrames;
            m_animationSpeed = animationSpeed;

            if (hasDirections) {
                // Assumes texture sprite sheet has directional sprites in Column 0, Rows 1-4 (Down, Right, Left, Up)
                Animation downAnimation = {0, numFrames, animationSpeed};
                Animation rightAnimation = {1, numFrames, animationSpeed};
                Animation leftAnimation = {2, numFrames, animationSpeed};
                Animation upAnimation = {3, numFrames, animationSpeed};

                m_animations.emplace("DownAnimation", downAnimation);
                m_animations.emplace("RightAnimation", rightAnimation);
                m_animations.emplace("LeftAnimation", leftAnimation);
                m_animations.emplace("UpAnimation", upAnimation);
                m_animationIndex = 0;
                m_currentAnimation = "DownAnimation";
            } else {
                Animation singleAnimation = {0, numFrames, animationSpeed};
                m_animations.emplace("SingleAnimation", singleAnimation);
                m_animationIndex = 0;
                m_currentAnimation = "SingleAnimation";
            }

            play(m_currentAnimation);

            setTexture(filePath);
        }

        void setTexture(std::string assetTextureId) {
            m_texture = Game::assetManager->getTexture(assetTextureId);
        }
        
        void play(std::string animationName) {
            m_numFrames = m_animations[animationName].numFrames;
            m_animationIndex = m_animations[animationName].index;
            m_animationSpeed = m_animations[animationName].animationSpeed;
            m_currentAnimation = animationName;
        }

        void initialize() override {
            m_transform = m_owner->getComponent<TransformComponent>();
            m_src = {0, 0, m_transform->getWidth(), m_transform->getHeight()};
        }

        void update(float deltaTime) override {
            if (m_isAnimated) {
                m_src.x = m_src.w * static_cast<int>((SDL_GetTicks() / m_animationSpeed) % m_numFrames);
            }
            m_src.y = m_animationIndex * m_transform->getHeight();

            m_dest = {  static_cast<int>(m_transform->getPosition().x) - (m_isFixed ? 0 : Game::camera.x), 
                        static_cast<int>(m_transform->getPosition().y) - (m_isFixed ? 0 : Game::camera.y),
                        m_transform->getWidth() * m_transform->getScale(),
                        m_transform->getHeight() * m_transform->getScale()
                    };
        }

        void render() override {
            TextureManager::draw(m_texture, m_src, m_dest, m_spriteFlip);
        }

};

#endif
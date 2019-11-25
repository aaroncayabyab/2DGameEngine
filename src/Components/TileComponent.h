#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"
#include "../Game.h"
#include "../AssetManager.h"

class TileComponent : public Component {
    private:
        SDL_Texture* m_texture;
        SDL_Rect m_srcRect;
        SDL_Rect m_destRect;
        glm::vec2 m_position;
    
    public:
        TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string textureId) {
            m_texture = Game::assetManager->getTexture(textureId);
            m_srcRect = { 
                        sourceRectX,
                        sourceRectY,
                        tileSize,
                        tileSize
                        };

            m_destRect = { 
                        x,
                        y,
                        tileSize * tileScale,
                        tileSize * tileScale
                        };
                        
            m_position = {x, y};
        }

        ~TileComponent() {
            SDL_DestroyTexture(m_texture);
        }

        void initialize() override {
        }

        void update(float deltaTime) override {
            m_destRect.x = m_position.x - Game::camera.x;
            m_destRect.y = m_position.y - Game::camera.y;
        }

        void render() override {
            TextureManager::draw(m_texture, m_srcRect, m_destRect, SDL_FLIP_NONE);
        }

};

#endif
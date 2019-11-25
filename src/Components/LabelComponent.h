#ifndef LABELCOMPONENT_H
#define LABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"

using namespace std;


class LabelComponent: public Component {
    private:
        SDL_Rect m_position;
        std::string m_text;
        std::string m_fontFamily;
        SDL_Color m_color;
        SDL_Texture* m_texture;

    public:
        LabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color) {
            m_position.x = x;
            m_position.y = y;
            m_text = text;
            m_fontFamily = fontFamily;
            m_color = color;

            setLabelText(text, fontFamily);
        }

        void setLabelText(std::string text, std::string fontFamily) {
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->getFont(fontFamily), text.c_str(), m_color);
            m_texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            SDL_FreeSurface(surface);

            SDL_QueryTexture(m_texture, nullptr, nullptr, &m_position.w, &m_position.h);

        }

        void initialize() override {
        }

        void update(float deltaTime) override {

        }

        void render() override {
            FontManager::draw(m_texture, m_position);
        }

};

#endif
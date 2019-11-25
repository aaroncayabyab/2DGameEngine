#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "./EntityManager.h"
#include "./TextureManager.h"
#include "./FontManager.h"

class AssetManager {
    private:
        EntityManager& m_manager;
        std::map<std::string, SDL_Texture*> m_textures;
        std::map<std::string, TTF_Font*> m_fonts;
    public:
        AssetManager(EntityManager& manager);
        ~AssetManager();
        void clearData();

        void addTexture(std::string textureId, const char* filePath);
        void addFont(std::string fontId, const char* filePath, int fontSize);
        SDL_Texture* getTexture(std::string textureId);
        TTF_Font* getFont(std::string fontId);

};

#endif;
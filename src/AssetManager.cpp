#include "./AssetManager.h"

AssetManager::AssetManager(EntityManager& manager): m_manager(manager) {}

void AssetManager::clearData() {
    m_textures.clear();
    m_fonts.clear();
}

void AssetManager::addTexture(std::string textureId, const char* filePath) {
    m_textures.emplace(textureId, TextureManager::loadTexture(filePath));
}

SDL_Texture* AssetManager::getTexture(std::string textureId) {
    return m_textures[textureId];
}

void AssetManager::addFont(std::string fontId, const char* filePath, int fontSize) {
    m_fonts.emplace(fontId, FontManager::loadFont(filePath, fontSize));
}

TTF_Font* AssetManager::getFont(std::string fontId) {
    return m_fonts[fontId];
}
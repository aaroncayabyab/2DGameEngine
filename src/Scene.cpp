#include <fstream>
#include "./Scene.h"
#include "./Game.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager manager;

Scene::Scene(std::string textureId, int scale, int tileSize): m_textureId(textureId), m_scale(scale), m_tileSize(tileSize) {}

void Scene::loadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    m_mapSize = { mapSizeX * m_scale * m_tileSize, mapSizeY * m_scale * m_tileSize };
    //Read map tile definitions from .map file
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int row = 0; row < mapSizeY; row++) {
        for (int col = 0; col < mapSizeX; col++) {
            char ch;

            // Get row # from map
            mapFile.get(ch);
            int mapIndexY = atoi(&ch) * m_tileSize;
            // Get column # from map
            mapFile.get(ch);
            int mapIndexX = atoi(&ch) * m_tileSize;
            //Ignore ','
            mapFile.ignore();
            addTile(mapIndexX, mapIndexY, col * m_scale * m_tileSize, row * m_scale * m_tileSize);
        }
    }

    mapFile.close();
}

void Scene::addTile(int sourceX, int sourceY, int x, int y) {
    //Add new tile entity in game scene
    Entity& newTile(manager.addEntity("Tile", LayerType::TILEMAP_LAYER));
    newTile.addComponent<TileComponent>(sourceX, sourceY, x, y, m_tileSize, m_scale, m_textureId);
}

glm::vec2 Scene::getMapSize() {
    return m_mapSize;
}

int Scene::getScale() {
    return m_scale;
}

int Scene::getTileSize() {
    return m_tileSize;
}


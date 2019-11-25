#ifndef SCENE_H
#define SCENE_H

#include <string>
#include"../lib/glm/glm.hpp"
class Scene {
    private:
        std::string m_textureId;
        glm::vec2 m_mapSize;
        int m_scale;
        int m_tileSize;
    public:
        Scene(std::string textureId, int scale, int tileSize);
        ~Scene();

        void loadMap(std::string filePath, int mapSizeX, int mapSizeY);
        void addTile(int sourceX, int sourceY, int x, int y);

        glm::vec2 getMapSize();
        int getScale();
        int getTileSize();
};

#endif
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/lua/sol.hpp"
#include "./Entity.h"
#include "./Component.h"
#include "./EntityManager.h"
#include "./AssetManager.h"

class AssetManager;

class Game {
    private:
        Entity* m_player;
        bool m_isRunning;
        SDL_Window *window;
    public:
        Game();
        ~Game();
        int ticksLastFrame;
        bool isRunning() const;
        void initialize(int width, int height);
        void processInput();
        void update();
        void render();
        void destroy();
        void loadLevel(int level);
        void handleCameraMovement();
        void checkCollisions();
        void processGameOver();
        void processNextLevel(int level);

        static SDL_Renderer *renderer;
        static SDL_Event event;
        static SDL_Rect camera;
        static AssetManager *assetManager;

};

#endif
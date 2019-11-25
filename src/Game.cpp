#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./Scene.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/InputComponent.h"
#include "./Components/LabelComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include"../lib/glm/glm.hpp"

EntityManager manager;
Scene* scene;

AssetManager* Game::assetManager = new AssetManager(manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

Game::Game() {
    this->m_isRunning = false;
    m_player = nullptr;
}

Game::~Game() {

}

bool Game::isRunning() const {
    return this->m_isRunning;
}

void Game::initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if(TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF." << std::endl;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }    

    // Loads LEVEL from Lua Config Script
    loadLevel(1);

    m_isRunning = true;
    return;
}

void Game::loadLevel(int level) {
    // Load Lua file
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(level);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    sol::table levelData = lua[levelName];

    /*  Load ASSETS from Lua Config file */
    sol::table levelAssets = levelData["assets"];
    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> currentAssetIndex = levelAssets[assetIndex];
        if (currentAssetIndex == sol::nullopt) break;

        sol::table asset = levelAssets[assetIndex];
        std::string assetType = asset["type"];

        if (assetType.compare("texture") == 0) {
            std::string assetId = asset["id"];
            std::string assetFile = asset["file"];
            assetManager->addTexture(assetId, assetFile.c_str());
        }

        if (assetType.compare("font") == 0) {
            std::string assetId = asset["id"];
            std::string assetFile = asset["file"];
            int fontSize = asset["fontSize"];
            assetManager->addFont(assetId, assetFile.c_str(), fontSize);
        }
        assetIndex++;
    }

    /*  Load MAP (Scene) from Lua Config file */
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];
    
    scene = new Scene(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    scene->loadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );

    /* Load ENTITIES & COMPONENTS from Lua Config file */
 sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        sol::optional<sol::table> currentEntityIndex = levelEntities[entityIndex];
        if (currentEntityIndex == sol::nullopt) {
            break;
        }
        else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            // Add new entity
            auto& newEntity(manager.addEntity(entityName, entityLayerType));

            // Add Transform Component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if (existsTransformComponent != sol::nullopt) {
                newEntity.addComponent<TransformComponent>(
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"]),
                    static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }

            // Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt) {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if (isAnimated) {
                    newEntity.addComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                        static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                        static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                        static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                } else {
                    newEntity.addComponent<SpriteComponent>(textureId, false);
                }
            }

            // Add input control component
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if (existsInputComponent != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                if (existsKeyboardInputComponent != sol::nullopt) {
                    std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                    std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                    std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                    std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                    std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                    newEntity.addComponent<InputComponent>(upKey, rightKey, downKey, leftKey, shootKey);
                }
            }

            // Add collider component
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if (existsColliderComponent != sol::nullopt) {
                std::string colliderTag = entity["components"]["collider"]["tag"];
                newEntity.addComponent<ColliderComponent>(
                    colliderTag,
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"])
                );
            }

            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                int projectileRange = entity["components"]["projectileEmitter"]["range"];
                int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                Entity& projectile(manager.addEntity("projectile", PROJECTILE_LAYER));
                projectile.addComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.addComponent<SpriteComponent>(textureAssetId);
                projectile.addComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.addComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }
        }
        entityIndex++;
    }


    // Set player entity
    m_player = manager.getEntityByName("player");
}

void Game::processInput() {
    SDL_PollEvent(&event);

    switch (event.type) {
        // Quit Keys
        case SDL_QUIT: {
            m_isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                m_isRunning = false;
            }
            break;
        }
        default: {
            break;
        }
    }
}

void Game::handleCameraMovement() {
    if (m_player == nullptr) return;

    TransformComponent* playerTransform = m_player->getComponent<TransformComponent>();
    camera.x = playerTransform->getPosition().x - (WINDOW_WIDTH / 2);
    camera.y = playerTransform->getPosition().y - (WINDOW_HEIGHT / 2);

    // Clamp camera
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;

    // Clamp player movement
    glm::vec2 maxBounds = { scene->getMapSize().x - (scene->getScale() * playerTransform->getWidth()), 
                            scene->getMapSize().y - (scene->getScale() * playerTransform->getHeight())
                          };
    if (playerTransform->getPosition().x < 0) playerTransform->setPosition({ 0, playerTransform->getPosition().y});
    if (playerTransform->getPosition().x > maxBounds.x ) playerTransform->setPosition({ maxBounds.x, playerTransform->getPosition().y});
    if (playerTransform->getPosition().y < 0) playerTransform->setPosition({ playerTransform->getPosition().x, 0 });
    if (playerTransform->getPosition().y > maxBounds.y) playerTransform->setPosition({ playerTransform->getPosition().x, maxBounds.y });

}

void Game::checkCollisions() {
    CollisionType collisionType = manager.checkCollisions();

    if(collisionType == PLAYER_ENEMY_COLLISION) {
        processGameOver();
    }
    else if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        processNextLevel(1);
    }
    else if (collisionType == PLAYER_PROJECTILE_COLLISION) {
        processGameOver();
    }
}

void Game::processGameOver() {
    std::cout << "Game over" << std::endl;
    m_isRunning = false;
}

void Game::processNextLevel(int level) {
    std::cout << "Level Complete" << std::endl;
    m_isRunning = false;
}



void Game::update() {
    // Wait until 16.67ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

    // Delta time is difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame)/1000.0f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets new ticks for current frame to be used in next pass
    ticksLastFrame = SDL_GetTicks();

    //Use deltaTime to update game objects
    manager.update(deltaTime);

    handleCameraMovement();
    checkCollisions();
    
    // manager.showCollisionBounds();
}

void Game::render() {
    // Set background color
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);

    // Clear back buffer
    SDL_RenderClear(renderer);

    // Draw game objects to scene
    if (manager.hasNoEntities()) {
        return;
    }

    manager.render();

    // swap front and back buffers
    SDL_RenderPresent(renderer);

}

void Game::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
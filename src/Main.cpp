#include <iostream>
#include "./Constants.h"
#include "./Game.h"

int main(int argc, char *args[]) {
    std::cout << "Game is running..." << std::endl;

    Game *game = new Game();

    game->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (game->isRunning()) {
        game->processInput();
        game->update();
        game->render();
    }

    std::cout << "Game closed." << std::endl;
    game->destroy();

    return 0;
}
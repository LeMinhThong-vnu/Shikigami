#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "global.h"
#include "util/draw.h"
#include "game.h"
#include "objects/baby.h"
#include "objects/player.h"
#include "objects/ghost.h"

Game* game = new Game();

int main(int argc, char* argv[]) {
    
    if (!game->init()) {
        return 1;
    }

    game->setPlayer(new Player(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, game));
    game->addObject(new Ghost(WINDOW_WIDTH * 1 / 5, WINDOW_HEIGHT * 1 / 5, game));
    // game->addObject(new Ghost(WINDOW_WIDTH * 4 / 5, WINDOW_HEIGHT * 4 / 5, game));
    // game->addObject(new Ghost(WINDOW_WIDTH * 1 / 5, WINDOW_HEIGHT * 4 / 5, game));
    // game->addObject(new Ghost(WINDOW_WIDTH * 4 / 5, WINDOW_HEIGHT * 1 / 5, game));
    game->set_baby(new Baby(game));

    Uint32 frameStart;
    int frameDelta;
    const int frameDelay = 1000 / FPS;

    while (!game->isQuit()) {

        frameStart = SDL_GetTicks();

        // Input
        game->handleInput();
        
        // Update
        game->update();

        // Render
        game->render();

        // Clean Deleted Game Objects
        game->cleanObjects();

        // Add new Objects
        game->addObjectBuffer();

        // Set FPS
        frameDelta = SDL_GetTicks() - frameStart;

        if (frameDelay > frameDelta) {
            SDL_Delay(frameDelay - frameDelta);
        }

    }
    
    // Destroy Window and Renderer after quit game
    // game->clean();
    delete game;

    return 0;
}
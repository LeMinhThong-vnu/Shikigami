#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
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

    game->setPlayer(new Player(WINDOW_WIDTH/2, WINDOW_HEIGHT * 2 / 3, game));
    game->set_baby(new Baby(game));

    Uint32 frameStart;
    int frameDelta;
    const int frameDelay = 1000 / FPS;

    while (!game->isQuit()) {

        frameStart = SDL_GetTicks();

        game->handleInput();
        
        game->update();

        game->render();

        game->cleanObjects();

        if (game->get_state() == GME_STE_GAME) {
            if (Game::get_tick() % 400 == 0) {
                double angle = (rand() % 360) * M_PI / 180;
                game->addObject(new Ghost(
                    (WINDOW_WIDTH / 2 - 100) * std::cos(angle) + (rand() % 100 - 50), 
                    (WINDOW_HEIGHT / 2 - 50) * std::sin(angle) + (rand() % 100 - 50),
                    game
                ));
            }
        }

        game->addObjectBuffer();

        frameDelta = SDL_GetTicks() - frameStart;

        if (frameDelay > frameDelta) {
            SDL_Delay(frameDelay - frameDelta);
        }

    }
    
    delete game;

    return 0;
}
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "global.h"
#include "util/draw.h"
#include "game_objects/player.h"
#include "game_objects/ghost.h"

bool quit = false;

void handle_events();
void update();
void render();
void clean();
void clean_game_objects();
void insert_game_object(GameObjectIndex ind);

Player* player = nullptr;

int main(int argc, char* argv[]) {
    
    if (!initApp()) {
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
        exit(1);
    }

    player = new Player(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    insert_game_object({ obj: player, type: GOT_Player });
    insert_game_object({ obj: new Ghost(0, WINDOW_WIDTH * 2 / 4, player), type: GOT_Ghost });

    Uint32 frameStart;
    int frameDelta;
    const int frameDelay = 1000 / FPS;

    while (!quit) {
        
        TICK++;

        frameStart = SDL_GetTicks();

        // Input
        handle_events();
        
        // Update
        update();

        // Render
        render();

        // Clean Deleted Game Objects
        clean_game_objects();

        // Set FPS
        frameDelta = SDL_GetTicks() - frameStart;

        if (frameDelay > frameDelta) {
            SDL_Delay(frameDelay - frameDelta);
        }

    }
    
    // Destroy Window and Renderer after quit game
    clean();

    return 0;
}

void insert_game_object(GameObjectIndex ind) {
    OBJ_ID++;
    game_objects.push_back(ind);
}

void handle_events() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;

        default:
            break;
    }

    player->handleInput(event);
}

void update() {
    //
    // player->update();
    for (GameObjectIndex goi : game_objects) {
        switch (goi.type) {
            case (GOT_Player):
                (dynamic_cast<Player*>(goi.obj))->update();
                break;
            case (GOT_Ghost):
                (dynamic_cast<Ghost*>(goi.obj))->update();
                break;
            default:
                break;
        }
    }
}

void clean_game_objects() {
    for (int i = 0; i < game_objects.size(); i++) {
        const GameObjectIndex goi = game_objects.at(i);
        if (goi.obj->getCleanFlag()) {
            delete goi.obj;
            game_objects.erase(game_objects.begin() + i);
            i--;
        }
    }
}

void render() {
    SDL_RenderClear(renderer);

    // Render
    // player->debug();
    // player->render();
    for (GameObjectIndex goi : game_objects) {
        switch (goi.type) {
            case (GOT_Player):
                (dynamic_cast<Player*>(goi.obj))->render();
                break;
            case (GOT_Ghost):
                (dynamic_cast<Ghost*>(goi.obj))->render();
                break;
        }
    }

    SDL_RenderPresent(renderer);
}

void clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Game Cleaned of Objects" << std::endl;
}
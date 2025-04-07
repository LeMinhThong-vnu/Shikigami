#include "global.h"
#include <iostream>

std::vector<GameObjectIndex> game_objects = {};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1200;

const int BG_R = 211;
const int BG_G = 211;
const int BG_B = 211;

int TICK = 0;

const int FPS = 60;

const char* TITLE = "Game";

bool initApp() {
    // Init Subsys
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "--- SUBSYS INIT ERROR ---" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        std::cout << "-------------------------" << std::endl;
        return false;
    }
    // Create Window
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "--- WINDOW INIT ERROR ---" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        std::cout << "-------------------------" << std::endl;
        return false;
    }
    std::cout << "Created Window!" << std::endl;
    // Create Renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::cout << "--- RENDERER INIT ERROR ---" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        std::cout << "---------------------------" << std::endl;
        return false;
    }
    std::cout << "Created Renderer!" << std::endl;
    SDL_SetRenderDrawColor(renderer, BG_R, BG_G, BG_B, 1);

    return true;
}
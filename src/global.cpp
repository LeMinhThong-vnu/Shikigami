#include "global.h"
#include <iostream>

// std::vector<GameObject*> game_objects = {};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int WINDOW_HEIGHT = 768;
const int WINDOW_WIDTH = 1024;

const int BG_R = 211;
const int BG_G = 211;
const int BG_B = 211;

const double GRV = 0.5;
const double TRC = 0.3;

const int FPS = 60;

const char* TITLE = "Game";
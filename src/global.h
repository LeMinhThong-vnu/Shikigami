#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL.h>
#include <vector>
#include "objects/game_object.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern std::vector<GameObject*> game_objects;

extern const int BG_R;
extern const int BG_G;
extern const int BG_B;

extern const double GRV;
extern const double TRC;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const char* TITLE;

extern const int FPS;

#endif
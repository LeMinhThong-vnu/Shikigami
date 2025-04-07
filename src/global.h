#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL.h>
#include <vector>
#include "game_objects/game_object.h"

enum GameObjectTypes {
    GOT_Player = 0,
    GOT_Ghost = 1
};

static int OBJ_ID = 0;

struct GameObjectIndex {
    int id;
    GameObject *obj;
    GameObjectTypes type;
};

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern std::vector<GameObjectIndex> game_objects;

extern const int BG_R;
extern const int BG_G;
extern const int BG_B;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const char* TITLE;

extern const int FPS;

extern int TICK;

bool initApp();

#endif
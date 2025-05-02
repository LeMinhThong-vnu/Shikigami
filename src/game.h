#ifndef GAME_H
#define GAME_H

#include "./util/struct_def.h"
#include "./util/enum_def.h"
#include "global.h"
#include <map>
#include <string>
#include <vector>
#include <SDL_ttf.h>

class Game {
    public:
        friend class UI;

        Game();
        ~Game();

        bool init();

        void update();

        void updateEnemy(Enemy* _obj);
        
        void renderEnemy(Enemy* _obj);

        void updateShikigami(Shikigami* _obj);

        void renderShikigami(Shikigami* _obj);

        void handleInput();

        void render();

        void debug();

        void clean();

        void remove_object(GameObject* goi);
        
        void free();

        void cleanObjects();

        void preload();

        static void addObject(GameObject* index);

        void addObjectBuffer();

        void setPlayer(Player* _player);

        void set_baby(Baby* _baby);

        Player* getPlayer();

        Baby* get_baby();

        void apply_collision();

        bool isQuit() { return quit; };

        void insertRenderBuffer(GameObject* goi, std::string layer_key);

        static int get_tick() { return Game::TICK; }

    private:
        static std::vector<GameObject*> objects;
        std::map<std::string, std::vector<GameObject*>> layers = {};
        Player* player = nullptr;
        Baby* baby = nullptr;
        static int TICK;
        bool quit = false;
        SDL_Texture* shadow = nullptr;
        SDL_Texture* background = nullptr;
        std::vector<GameObject*> obj_buffer = {};
        UI* ui = nullptr;
};

#endif
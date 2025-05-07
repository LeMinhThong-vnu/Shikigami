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

        void render_background();

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

        UI* get_ui() { return ui; }

        void apply_collision();
        
        void update_game();

        void update_title();

        void update_pause();

        void update_gameover();

        void update_start();

        void update_tutorial();

        bool isQuit() { return quit; };

        void insertRenderBuffer(GameObject* goi, std::string layer_key);

        static int get_tick() { return Game::TICK; }

        static void set_hitstop(int n) { Game::HITSTOP = n; }
        
        static void set_screenshake(int n) { Game::HITSTOP = n; }

        GAME_STATES get_state() { return state; }

        void set_state(GAME_STATES _state) { state = _state; }

    private:
        static std::vector<GameObject*> objects;
        std::map<std::string, std::vector<GameObject*>> layers = {};
        Player* player = nullptr;
        Baby* baby = nullptr;
        static int TICK;
        static int HITSTOP;
        static int SCREENSHAKE;
        bool quit = false;
        SDL_Texture* shadow = nullptr;
        SDL_Texture* background = nullptr;
        TweenComponent* tweens = nullptr;
        std::vector<GameObject*> obj_buffer = {};
        UI* ui = nullptr;
        GAME_STATES state = GAME_STATES::GME_STE_TITLE;
        
        // Tutorial
        bool tutorial_summon_flag = false;
        bool tutorial_throw_flag = false;
        bool tutorial_recycle_flag = false;
};

#endif
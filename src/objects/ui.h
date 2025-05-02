#ifndef UI_H
#define UI_H

#include "player.h"
#include "../util/enum_def.h"
#include "../global.h"
#include "../components/sprite_component.h"
// #include "../game.h"

class UI {
    public:
        UI(Game* _game);
        ~UI();

        void update();

        void render();

        GAME_STATES getState() { return state; }
        void setState(GAME_STATES _state) { state = _state; }

    private:

        void render_game();
        void update_game();

        void render_pause();
        void update_pause();

        void render_start();
        void update_start();

        void render_gameover();
        void update_gameover();

        void render_settings();
        void update_settings();

        SpriteComponent* sprite;

        Game* game;

        GAME_STATES state = GME_STE_GAME;
};

#endif
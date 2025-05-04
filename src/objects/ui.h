#ifndef UI_H
#define UI_H

#include "player.h"
#include "../util/enum_def.h"
#include "../global.h"
#include "../components/sprite_component.h"
// #include "../game.h"

class UI {
    public:
        friend class Game;
        friend class Player;

        UI(Game* _game);
        ~UI();

        void update();

        void render();

    private:

        void render_game();
        void update_game();

        void render_pause();
        void update_pause();

        void render_start();
        void update_start();

        void render_gameover();
        void update_gameover();

        void render_title();
        void update_title();

        SpriteComponent* sprite;

        TweenComponent* tweens;

        Game* game;

        SDL_Texture* txtr_text = nullptr;
};

#endif
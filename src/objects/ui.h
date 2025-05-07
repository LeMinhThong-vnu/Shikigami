#ifndef UI_H
#define UI_H

#include <map>
#include <cstring>
#include "player.h"
#include "../util/enum_def.h"
#include "../global.h"
#include "../components/sprite_component.h"

class UI {
    public:
        friend class Game;
        friend class Player;

        UI(Game* _game);
        ~UI();

        void update();

        void render();

        TweenComponent* get_tweens() { return tweens; }

    private:

        void render_game();

        void render_pause();

        void render_start();

        void render_gameover();

        void render_title();

        void render_tutorial();

        void print_text(std::string key, int x, int y);
        void print_text(std::string key, int x, int y, double sx, double sy);
        void print_text(std::string key, int x, int y, double sx, double sy, double alpha);
        void print_text(std::string key, int x, int y, double sx, double sy, double alpha, double ox, double oy);

        SpriteComponent* sprite;

        TweenComponent* tweens;

        Game* game;

        SDL_Texture* text_texture = nullptr;

        std::map<std::string, AtlusFrame> text_atlus = {};
};

#endif
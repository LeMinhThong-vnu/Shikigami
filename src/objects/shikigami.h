#ifndef SHIKIGAMI_H
#define SHIKIGAMI_H

#include "../util/class_def.h"
#include <SDL_mixer.h>
#include "game_object.h"

class Shikigami : public GameObject {
    public:
        Shikigami(int _x, int _y, SHIKIGAMI_TYPES _type, Game* _game);
        ~Shikigami();

        virtual void grabbed();

        virtual void thrown(double _angle);

        virtual void update();

        virtual void update_idle();

        virtual void update_grabbed();

        virtual void update_thrown();

        virtual void update_attack();

        virtual void update_wait();

        virtual void update_despawn();

        virtual void attack();

        SHIKIGAMI_TYPES get_shiki_type() { return shiki_type; };

        SHIKIGAMI_STATES get_state() { return state; }

        int get_cost() { return cost; }

    protected:
        bool grabable = false;
        SHIKIGAMI_STATES state = SHK_STE_IDLE;
        SHIKIGAMI_TYPES shiki_type;
        int life_max = 500;
        int life = 500;
        int damage = 20;
        int cooldown = 0;
        int cooldown_max = 50;
        int cost = 1;
        Enemy* target = nullptr;
        Mix_Chunk* sfx = nullptr;
};

#endif
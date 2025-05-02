#ifndef ENEMY_H
#define ENEMY_H

#include "../util/class_def.h"
#include "game_object.h"

class Enemy : public GameObject {
    public:
        Enemy(int _x, int _y, ENEMY_TYPES _enemy_type, Game* _game);
        ~Enemy();

        virtual void grabbed();

        virtual void thrown(double _angle, double _v);

        virtual void update_grabbed();

        virtual void update_thrown();

        virtual void update_knockdown();

        virtual void update_dead();

        virtual void update_attack();

        virtual void add_damage(int damage) {
            hp -= damage;
        }

        ENEMY_STATES getState() { return state; }

        ENEMY_TYPES getEnemyType() { return enemy_type; }

    protected:
        bool grabable = false;
        ENEMY_STATES state = ENM_STE_IDLE;
        ENEMY_TYPES enemy_type;
        int stun = 0;
        int hp = 100;
        int max_hp = 100;
        int cooldown = 0;
        int cooldown_max = 100;
};

#endif
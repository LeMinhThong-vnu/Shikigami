#ifndef GHOST_H
#define GHOST_H

#include "enemy.h"

class Ghost : public Enemy {
    public:
        Ghost(int _x, int _y, Game* _game);
        ~Ghost();

        void update();

        void update_idle();

        void update_attack();

    private:
        double move_speed = 1;
};

#endif
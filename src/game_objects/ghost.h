#ifndef GHOST_H
#define GHOST_H

#include "enemy.h"
#include <tweeny.h>

enum GHOST_STATE {
    GS_IDLE = 1,
    GS_DIE = 2,
    GS_GRABBED = 3,
    GS_THROWN = 4,
};

class Ghost : public Enemy {
    public:
        Ghost(int _x, int _y, Player *p);
        ~Ghost();

        void update();

        void render();

        void seal();
        
        void grab();

        void thrown(double angle);
    private: 
        Sprite *sprite;
        int move_speed = 1.3;
        double dx = 0;
        double dy = 0;
        double vx = 0;
        double vy = 0;
        double traction = 0.3;
        int facing = 1;
        GHOST_STATE state = GS_IDLE;
        Player *player;
        tweeny::tween<double> tween;
};

#endif
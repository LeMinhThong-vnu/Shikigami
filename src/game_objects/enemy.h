#ifndef ENEMY_H 
#define ENEMY_H

#include "player.h"
#include "sprite.h"
#include "game_object.h"

class Enemy : public GameObject {
    public:
        Enemy();
        ~Enemy();

        void clean();

    private:
        double xv = 0;
        double yv = 0;
        double traction = 0.3;
        int move_speed = 3;
        Sprite *sprite;
        Player *player;
    
};

#endif
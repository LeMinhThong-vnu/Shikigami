#ifndef BABY_H
#define BABY_H

#include "game_object.h"
#include "../util/class_def.h"

class Baby : public GameObject {

    public:
        Baby(Game* _game);
        ~Baby();

        void update();

        void damage(int dmg);

    private:
        int hp = 200;
        int hp_max = 200;
        double step = 50;
        double step_max = 50;
};

#endif
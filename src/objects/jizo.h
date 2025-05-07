#ifndef SHIKI_JIZO_H
#define SHIKI_JIZO_H

#include "shikigami.h"

class Jizo : public Shikigami {
    public:
        Jizo(int _x, int _y, Game* _game);
        ~Jizo();

        void update();

        void update_attack();

        void update_wait();

        void update_idle();

        void attack();

        void thrown(double _angle);

    private:
        Enemy* target = nullptr;
        
};

#endif
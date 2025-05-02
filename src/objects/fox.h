#ifndef SHIKI_FOX_H
#define SHIKI_FOX_H

#include "shikigami.h"

class Fox : public Shikigami {
    public:
        Fox(int _x, int _y, Game* _game);
        ~Fox();

        void update();

        void update_attack();

        void update_wait();

        void update_idle();

        void attack();

    private:
        
};

#endif
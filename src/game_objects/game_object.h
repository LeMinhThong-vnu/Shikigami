#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL.h>
#include <tween.h>
#include "sprite.h"
#include <SDL_image.h>
#include "../util/draw.h"
#include "tween_object.h"

class GameObject {
    public:
        GameObject();
        ~GameObject();

        virtual void render();

        virtual void update();

        // GET
        double getX() { return x; }
        double getY() { return y; }

        int getFacing() { return facing; }

        void setPos(double _x, double _y) {
            x = _x;
            y = _y;
        }
        void setX(double _x) { x = _x; }
        void setY(double _y) { y = _y; }

        void setFacing(int f) { facing = f; }

        bool getCleanFlag() { return clean_flag; }
        void setCleanFlag(bool flag) { clean_flag = flag; }

        void collision();

        virtual void clean() {};

    protected:
        double x = 0;
        double y = 0;
        float angle = 0;
        int facing = 1;
        Sprite *sprite;
        bool clean_flag = false;
        TweenObject *tween_object;
};

#endif
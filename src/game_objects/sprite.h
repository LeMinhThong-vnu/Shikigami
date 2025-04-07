#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <SDL.h>
#include <string>
#include <vector>
#include "../util/draw.h"
#include <map>

struct Frame {
    int index;
    int duration;
};

class Sprite {
    public:
        Sprite();
        ~Sprite();
    
        bool loadTexture( const char* path );

        void free();

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        void setAlpha( Uint8 _alpha );
        double getAlpha() { return alpha; };
        
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        void setScale(double _x, double _y) {
            sx = _x;
            sy = _y;
        }
        void setScaleX(double _x) { sx = _x; }
        void setScaleY(double _y) { sy = _y; }

        void setOrigin(double _x, double _y) {
            ox = _x;
            oy = _y;
        }
        void setOriginX(double _x) { ox = _x; }
        void setOriginY(double _y) { oy = _y; }

        void setAngle(double _angle) { angle = _angle; }

        int getWidth() { return w; };
        int getHeight() { return h; };

        void setFlip(SDL_RendererFlip _flip) { flip = _flip; };
        SDL_RendererFlip getFlip() { return flip; };

        void setAnim(std::string key) {
            frame_cur = 0;
            frame_sub = 0;
            anim_key = key;
            anim_length = (*animations)[anim_key].size();
            complete = false;
        }

        void update();

        bool isComplete() { return complete; }

    private:
        double sx = 1;
        double sy = 1;
        double ox = 0.5;
        double oy = 1;
        double alpha = 1;
        double angle = 0;
        int anim_length = 0;
        int w;
        int h;
        int cols;
        int rows;
        bool complete = false;
        std::string anim_key;
        int frame_sub = 0;
        int frame_cur = 0;
        std::map<std::string, std::vector<Frame>> *animations;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        SDL_Texture* texture = nullptr;
        
};

#endif
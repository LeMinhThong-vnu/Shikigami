#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "../util/struct_def.h"

class SpriteComponent {
    public:
        SpriteComponent();
        ~SpriteComponent();

        bool loadTexture( const char* path );

        void free();

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        void setAlpha( Uint8 _alpha );
        double getAlpha() { return alpha; };
        
        void render( int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* clip = NULL );

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

        void setDifference(double _x, double _y) {
            dx = _x;
            dy = _y;
        }
        void setDifferenceX(double _x) {
            dx = _x;
        }
        void setDifferenceY(double _y) {
            dy = _y;
        }
        
        double getDifferenceX() {
            return dx;
        }
        double getDifferenceY() {
            return dy;
        }

        void setAngle(double _angle) { angle = _angle; }
        double getAngle() { return angle; }

        void setDepth(int _depth) { depth = _depth; }
        int getDepth() { return depth; }

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
        
        void setAnim(std::string key, bool repeat) {
            if (repeat || anim_key != key) {
                frame_cur = 0;
                frame_sub = 0;
                anim_key = key;
                anim_length = (*animations)[anim_key].size();
                complete = false;
            }
        }

        std::string get_key() { return anim_key; }

        void update();

        bool isComplete() { return complete; }

    private:
        // Render
        double sx = 1;
        double sy = 1;
        double ox = 0.5;
        double oy = 1;
        double dx = 0;
        double dy = 0;
        double dz = 0;
        double alpha = 1;
        double angle = 0;
        int depth = 0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Texture
        int w;
        int h;
        int cols;
        int rows;
        SDL_Texture* texture = nullptr;
        std::map<std::string, std::vector<Frame>> *animations;

        // Animation
        std::string anim_key;
        int frame_sub = 0;
        int frame_cur = 0;
        bool complete = false;
        int anim_length = 0;
        
};

#endif
#include <SDL.h>

class CollisionBox {
    public:
        CollisionBox(int w, int h);
        ~CollisionBox();

        SDL_Rect* getRect() { return rect; }
        void setRect(int w, int h) {
            rect->w = w;
            rect->h = h;
        }

        // bool isColliding(SDL_Rect* r) {
        //     return (

        //     );
        // }

    private:
        SDL_Rect* rect;
};
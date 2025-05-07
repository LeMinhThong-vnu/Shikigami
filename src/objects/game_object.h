#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../components/physics_component.h"
#include "../components/sprite_component.h"
#include "../components/tween_component.h"
#include "../game.h"

class GameObject {
    public:
        GameObject(int _w, int _h, int _x, int _y, Game* _game);
        virtual ~GameObject();

        virtual void update();

        virtual void stateMachine();

        virtual void render();

        virtual void debug();

        virtual void clean();
        
        std::vector<GameObject*> getObjBuffer();

        void clearObjBuffer();

        void setRemove(bool _r) { remove = _r; }

        bool getRemove() { return remove; }

        PhysicsComponent* getBody() { return body; }

        SpriteComponent* getSprite() { return sprite; }

        int getFacing() { return facing; }

        bool isEnemy() { return (type == ENEMY); }

        GAME_OBJECT_TYPES getType() { return type; }

        bool is_grabbable() { return grabbable; }

        TweenComponent* get_tweens() { return tweens; }

    protected:
        int id = 0;
        static int MAX_ID;
        GAME_OBJECT_TYPES type;
        Game* game = nullptr;

        bool static_depth = false;

        PhysicsComponent* body = nullptr;
        SpriteComponent* sprite = nullptr;
        TweenComponent* tweens = nullptr;

        int move_x, move_y = 0;
        int facing = 1;

        bool remove = false;
        bool is_enemy = false;
        bool grabbable = false;
        std::vector<GameObject*> obj_buffer = {};
};

#endif
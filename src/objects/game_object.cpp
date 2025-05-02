#include "game_object.h"
#include "../global.h"

int GameObject::MAX_ID = 0;

GameObject::GameObject(int _w, int _h, int _x, int _y, Game* _game) {
    body = new PhysicsComponent(_w, _h, _x, _y, this);
    tweens = new TweenComponent();
    sprite = new SpriteComponent();
    game = _game;
}

GameObject::~GameObject() {
    clean();
}


void GameObject::clean() {
    printf("clean\n");
    delete sprite;
    delete body;
    delete tweens;
    for (GameObject* go : obj_buffer) {
        printf("delete obj\n");
        delete go;
    }
}

void GameObject::update() {
    if (body->isActive()) body->realign();
    body->clearColliding();
    body->applyForces();
    body->clamp(body->getW() / 2, body->getH(), WINDOW_WIDTH - body->getW() / 2, WINDOW_HEIGHT);
    if (!static_depth) sprite->setDepth(body->getY());
}

void GameObject::render() {
    sprite->render(body->getX(), body->getY() + body->getZ(), (facing == 1 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL), NULL);
}

void GameObject::stateMachine() {

}

void GameObject::debug() {

}

std::vector<GameObject*> GameObject::getObjBuffer() {
    return obj_buffer;
}

void GameObject::clearObjBuffer() {
    obj_buffer.clear();
}
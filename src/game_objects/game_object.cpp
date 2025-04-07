#include "../global.h"
#include "../util/draw.h"
#include "game_object.h"
#include <iostream>

GameObject::GameObject() {}
GameObject::~GameObject() {}

void GameObject::update() {
    // sprite->update();
}

void GameObject::render() {
    // sprite->render(x, y, NULL, 0.0, NULL, (facing == 1) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void GameObject::collision() {
    
}
#include "tween_component.h"

TweenComponent::TweenComponent() {

}

TweenComponent::~TweenComponent() {
    for (auto pair : tweens) {
        delete pair.second;
    }
    tweens.clear();
}

TweenObject* TweenComponent::get_tween(std::string name) {
    if (tweens.find(name) != tweens.end()) {
        TweenObject* tween = tweens.find(name)->second;
        return tween;
    }
    else return nullptr;
}

void TweenComponent::add_tween(std::string name, TweenObject* tween) {
    tweens.insert({name, tween});
}

void TweenComponent::update() {
    for (auto pair : tweens) {
        pair.second->step();
    }
}

void TweenComponent::clean_tweens() {
    for (auto pair : tweens) {
        if (pair.second->isComplete()) {
            tweens.erase(pair.first);
            delete pair.second;
        }
    }
}
#ifndef TWEEN_COMPONENT_H
#define TWEEN_COMPONENY_H

#include <map>
#include "tween_object.h"
#include <string>

class TweenComponent {
    public:
        TweenComponent();
        ~TweenComponent();

        TweenObject* get_tween(std::string name);

        void add_tween(std::string name, TweenObject* tween);

        void update();

        void clean_tweens();

    private:
        std::map<std::string, TweenObject*> tweens = {};
};

#endif
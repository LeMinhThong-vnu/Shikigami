#include "tween_object.h"
#include <tweeny.h>

TweenObject::TweenObject() {

}

TweenObject::~TweenObject() {
    
}

double TweenObject::value() {
    return cur_tween.peek();
}

void TweenObject::step() {
    if (tweens.size() > 0) {
        TweenIndex index = tweens.at(0);
        cur_tween.step(1);
        if (cur_tween.progress() == 1.0f) {
            tweens.erase(tweens.begin());
            complete = (tweens.size() == 0);
            setTween(index.type);
        }
    }
}

void TweenObject::clear() {
    tweens.clear();
}

void TweenObject::setTween(TWEEN_TYPES type) {
    switch (type) {
        case TWEEN_TYPES::IN:
            cur_tween = tweeny::from(-100.0).to(0.0).during(10).via(tweeny::easing::quinticIn);
            break;
        case TWEEN_TYPES::OUT:
            cur_tween = tweeny::from(-100.0).to(0.0).during(10).via(tweeny::easing::quinticOut);
            break;
        case TWEEN_TYPES::IN_OUT:
            cur_tween = tweeny::from(-100.0).to(0.0).during(10).via(tweeny::easing::quinticInOut);
            break;
        case TWEEN_TYPES::LINEAR:
            cur_tween = tweeny::from(-100.0).to(0.0).during(10).via(tweeny::easing::linear);
            break;
        default:
            cur_tween = tweeny::from(-100.0).to(0.0).during(10).via(tweeny::easing::linear);
            break;
    }
}

void TweenObject::add(double start, double end, int duration, TWEEN_TYPES type) {
    complete = false;
    tweens.push_back({
        start: start,
        end: end,
        duration: duration,
        type: type
    });
    setTween(type);
}

double TweenObject::value() {
    
}
#include "tween_object.h"
#include <tweeny.h>

TweenObject::TweenObject() {

}

TweenObject::~TweenObject() {
    tweens.clear();
}

double TweenObject::value() {
    return cur_tween.peek();
}

void TweenObject::step() {
    // printf("step_1\n");
    if (tweens.size() > 0) {
        // printf("step_2\n");
        flag = "";
        cur_tween.step(1);
        // printf("step_3\n");
        if (cur_tween.progress() == 1.0f) {
            // printf("step_4\n");
            tweens.erase(tweens.begin());
            complete = (tweens.size() == 0);
            if (!complete) {
                TweenIndex index = tweens.at(0);
                setTween(index);
            }
            // printf("step_5\n");
        }
    }
}

void TweenObject::clear() {
    tweens.clear();
}

void TweenObject::setTween(TweenIndex index) {
    switch (index.type) {
        case TWEEN_TYPES::IN:
            cur_tween = (tweeny::from(index.start).to(index.end).during(index.duration).via(tweeny::easing::cubicIn));
            break;
        case TWEEN_TYPES::OUT:
            cur_tween = (tweeny::from(index.start).to(index.end).during(index.duration).via(tweeny::easing::cubicOut));
            break;
        case TWEEN_TYPES::IN_OUT:
            cur_tween = tweeny::from(index.start).to(index.end).during(index.duration).via(tweeny::easing::cubicInOut);
            break;
        case TWEEN_TYPES::LINEAR:
            cur_tween = tweeny::from(index.start).to(index.end).during(index.duration).via(tweeny::easing::linear);
            break;
        default:
            cur_tween = tweeny::from(index.start).to(index.end).during(index.duration).via(tweeny::easing::linear);
            break;
    }
    flag = index.flag;
    // std::cout << flag << std::endl;
    cur_tween.step(1);
}

void TweenObject::add(double _start, double _end, int _duration, TWEEN_TYPES _type, std::string _flag) {
    complete = false;
    TweenIndex _index = {
        start: _start,
        end: _end,
        duration: _duration,
        type: _type,
        flag: _flag
    };
    if (tweens.size() == 0) setTween(_index);
    tweens.push_back(_index);
}

void TweenObject::add(double _start, double _end, int _duration, TWEEN_TYPES _type) {
    complete = false;
    TweenIndex _index = {
        start: _start,
        end: _end,
        duration: _duration,
        type: _type,
        flag: ""
    };
    if (tweens.size() == 0) setTween(_index);
    tweens.push_back(_index);
}

void TweenObject::add(TweenIndex _index) {
    complete = false;
    if (tweens.size() == 0) setTween(_index);
    tweens.push_back(_index);
}
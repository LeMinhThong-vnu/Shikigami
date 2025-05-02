#ifndef TWEEN_OBJECT_H
#define TWEEN_OBJECT_H

#include <iostream>
#include <tween.h>
#include <vector>
#include "../util/enum_def.h"
#include "../util/struct_def.h"

class TweenObject {
    public:
        TweenObject();
        ~TweenObject();

        void step();
        void add(double _start, double _end, int _duration, TWEEN_TYPES _type, std::string _flag);
        void add(double _start, double _end, int _duration, TWEEN_TYPES _type);
        void add(TweenIndex _index);
        void clear();
        void pause() { start = false; }
        void resume() { start = true; }
        double value();
        void setTween(TweenIndex type);
        bool isComplete() { return complete; }
        std::string get_flag() { return flag; }

    private:
        std::vector<TweenIndex> tweens = {};
        tweeny::tween<double> cur_tween;
        bool complete = true;
        bool start = false;
        std::string flag = "";
};

#endif
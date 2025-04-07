#ifndef TWEEN_OBJECT_FLAG
#define TWEEN_OBJECT_FLAG

#include <iostream>
#include <tween.h>
#include <vector>

enum TWEEN_TYPES {
    LINEAR = 1,
    IN     = 2,
    OUT    = 3,
    IN_OUT = 4,
};

struct TweenIndex {
    double start, end;
    int duration;
    TWEEN_TYPES type;
};

class TweenObject {
    public:
        TweenObject();
        ~TweenObject();

        void step();
        void add(double start, double end, int duration, TWEEN_TYPES type);
        void clear();
        void pause() { start = false; }
        void resume() { start = true; }
        double value();
        void setTween(TWEEN_TYPES type);
        bool isComplete() { return complete; }

    private:
        std::vector<TweenIndex> tweens = {};
        tweeny::tween<double> cur_tween;
        bool complete = true;
        bool start = false;
};

#endif
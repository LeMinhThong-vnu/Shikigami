#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H

#include "enum_def.h"
#include "class_def.h"
#include <string>

class GameObject;

struct Frame {
    int index;
    int duration;
};

struct AngleBetween {
    double  x_from, y_from,
            x_to, y_to,
            angle;
};

struct DistanceBetween {
    PhysicsComponent* body;
    double distance;
};

struct TweenIndex {
    double start, end;
    int duration;
    TWEEN_TYPES type;
    std::string flag;
};

struct AtlusFrame {
    int x, y,
        w, h;
};

#endif
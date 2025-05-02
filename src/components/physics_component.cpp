#include "physics_component.h"

PhysicsComponent::PhysicsComponent(int _w, int _h, double _x, double _y, GameObject* _owner) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    owner = _owner;
    grv = GRV;
    trc = TRC;
}

PhysicsComponent::~PhysicsComponent() {
    colliding.clear();
    distances.clear();
}

void PhysicsComponent::resetForces() {
    trc = TRC;
    grv = GRV;
    active = true;
    apply_collision = true;
}


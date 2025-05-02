#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <iostream>
#include <cmath>
#include <set>
#include "../util/struct_def.h"
#include "../global.h"
// #include "../objects/game_object.h"

class PhysicsComponent {
    public:
        PhysicsComponent(int _w, int _h, double _x, double _y, GameObject* _owner);
        ~PhysicsComponent();

        // Set position
        void setPos(double _x, double _y) {
            x = _x;
            y = _y;
        }
        void setPos(double _x, double _y, double _z) {
            x = _x;
            y = _y;
            z = _z;
        }
        void setPosX(double _x) {
            x = _x;
        }
        void setPosY(double _y) {
            y = _y;
        }
        void setPosZ(double _z) {
            z = _z;
        }
        void clamp(double min_x, double min_y, double max_x, double max_y) {
            x = std::min(std::max(x, min_x), max_x);
            y = std::min(std::max(y, min_y), max_y);
        }
        void clampX(double min_x, double max_x) {
            x = std::min(std::max(x, min_x), max_x);
        }
        void clampY(double min_y, double max_y) {
            y = std::min(std::max(y, min_y), max_y);
        }

        // Get position
        double getX() { return x; };
        double getY() { return y; };
        double getZ() { return z; };

        // Set size
        void setSize(int _w, int _h) {
            w = _w;
            h = _h;
        }

        // Get size
        int getW() { return w; }
        int getH() { return h; }

        // Set velocity
        void setVelocityAngle(double _v, double _a) {
            double _rad = _a * M_PI / 180;
            double _vx = _v * std::cos(_rad);
            double _vy = _v * std::sin(_rad);
            vx = _vx;
            vy = -_vy;
        }
        void setVelocityAngle(double _x, double _y, double _a) {
            double _rad = _a * M_PI / 180;
            double _vx = _x * std::cos(_rad);
            double _vy = _y * std::sin(_rad);
            vx = _vx;
            vy = -_vy;
        }
        void setVelocity(double _x, double _y) {
            vx = _x;
            vy = _y;
        }
        void setVelocity(double _x, double _y, double _z) {
            vx = _x;
            vy = _y;
            vz = _z;
        }
        void setVelocityX(double _x) {
            vx = _x;
        }
        void setVelocityY(double _y) {
            vy = _y;
        }
        void setVelocityZ(double _z) {
            vz = _z;
        }
        void setTraction(double _t) {
            trc = _t;
        }
        double getTraction() { return trc; }
        void setGravity(double _g) {
            grv = _g;
        }
        double getGravity() { return grv; }
        void addVelocity(double _x, double _y) {
            vx += _x;
            vy += _y;
        }
        void addVelocity(double _x, double _y, double _z) {
            vx += _x;
            vy += _y;
            vz += _z;
        }
        void addVelocityX(double _x) {
            vx += _x;
        }
        void addVelocityY(double _y) {
            vy += _y;
        }
        void addVelocityZ(double _z) {
            vz += _z;
        }
        void addVelocityAngle(double _v, double _a) {
            double _rad = _a * M_PI / 180;
            double _vx = _v * std::cos(_rad);
            double _vy = _v * std::sin(_rad);
            vx += _vx;
            vy -= _vy;
        }
        void applyTraction() {
            vx -= vx * trc;
            vy -= vy * trc;
        }
        void applyGravity() {
            vz += grv;
        }
        void applyForces() {
            applyTraction();
            applyGravity();
            x += vx;
            y += vy;
            z += vz;
            grounded = (z >= 0);
            if (z >= 0) {
                vz = 0;
                z = 0;
            }
        }

        void resetForces();

        // Get velocity
        double getVelocityX() { return vx; }
        double getVelocityY() { return vy; }
        double getVelocityZ() { return vz; }

        // Detection
        void setActive(bool _active) {
            active = _active;
        }
        bool isActive() { return active; };
        bool isApplyingCollision() { return apply_collision; }
        void setApplyingCollision(bool _ac) { apply_collision = _ac; }
        bool isColliding(PhysicsComponent* body) {
            if (!body->isActive()) return false;
            // Get self
            double self_left = x - (w / 2);
            double self_right = x + (w / 2);
            double self_top = y - (h / 2);
            double self_bottom = y + (h / 2);
            // Get body
            double body_left = body->x - (body->w / 2);
            double body_right = body->x + (body->w / 2);
            double body_top = body->y - (body->h / 2);
            double body_bottom = body->y + (body->h / 2);
            // Detect
            // printf("self:\n- sl: %lf\n- sr: %lf\n- st: %lf\n- sb: %lf\n", self_left, self_right, self_top, self_bottom);
            // printf("body:\n- bl: %lf\n- br: %lf\n- bt: %lf\n- bb: %lf\n", body_left, body_right, body_top, body_bottom);
            bool is_colliding = (
                self_left < body_right && 
                self_right > body_left &&
                self_top < body_bottom && 
                self_bottom > body_top 
            );
            if (is_colliding) {
                colliding.insert(body);
                if (active) body->colliding.insert(this);
            }
            return is_colliding;
        }
        void realign() {
            if (active && apply_collision) {
                for (PhysicsComponent* body : colliding) {
                    realignSingle(body);
                }
            }
        }
        std::set<PhysicsComponent*> getColliding() {
            return colliding;
        }
        void realignSingle(PhysicsComponent* _body) {
            if (active && apply_collision) {
                if (_body->getX() == x || _body->getY() == y) {
                    x += rand() % 2 + 1;
                    y += rand() % 2 + 1;
                }
                AngleBetween ang_btw = angleFrom(_body);
                addVelocityAngle(-1.5, ang_btw.angle);
            }
        }
        void clearColliding() {
            colliding.clear();
        }
        double getDistance(PhysicsComponent* body) {
            double _dx = body->x - x;
            double _dy = body->y - y;
            double _d = std::sqrt(std::pow(_dx, 2) + std::pow(_dy, 2));
            return _d;
        }
        void setDistances(PhysicsComponent* body) {
            double _d = getDistance(body);
            insertSortDistance(DistanceBetween({
                body: body,
                distance: _d
            }));
            body->insertSortDistance(DistanceBetween({
                body: this,
                distance: _d
            }));
        }
        std::vector<DistanceBetween> getDistances() {
            return distances;
        }
        void clearDistances() {
            distances.clear();
        }

        // Utility
        AngleBetween angleFrom(const PhysicsComponent* body) {
            double dx = body->x - x;
            double dy = body->y - y;
            double _dx = abs(dx);
            double _dy = abs(dy);
            double angle = ((_dx == 0) ? ((dy > 0) ? 90 : 270) : std::atan(_dy / _dx) * 180 / M_PI);
            if (dx < 0 && dy < 0) angle = 180 - angle;
            if (dx < 0 && dy > 0) angle = 180 + angle;
            if (dx > 0 && dy > 0) angle = 360 - angle;
            if (dx == 0) {
                angle = ((dy > 0) ? 270 : 90);
            }
            return AngleBetween({
                x_from: body->x,
                y_from: body->y,
                x_to: x,
                y_to: y,
                angle: angle
            });
        }
        GameObject* getOwner() { return owner; }

    private:

        void insertSortDistance(DistanceBetween dist_btwn) {
            int ind = 0;
            for (const DistanceBetween _dist : distances) {
                if (_dist.distance >= dist_btwn.distance) {
                    break;
                }
                ind++;
            }
            distances.insert(distances.begin() + ind, dist_btwn);
        }

        // Misc
        GameObject* owner = nullptr;
        // Position
        double x, y, z = 0;
        bool grounded = true;
        // Velocity
        double vx, vy, vz = 0;
        double trc;
        double grv;
        // Size
        int w, h;
        // Detection
        bool active = true;
        bool apply_collision = true;
        std::set<PhysicsComponent*> colliding = {};
        std::vector<DistanceBetween> distances = {};
};

#endif
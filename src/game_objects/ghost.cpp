#include "../global.h"
#include "ghost.h"
#include "player.h"
#include <cmath>

Ghost::Ghost(int _x, int _y, Player *p) {
    x = _x;
    y = _y;
    player = p;
    sprite = new Sprite();
    sprite->loadTexture("ghost");
    sprite->setAnim("idle");
}

Ghost::~Ghost() {
    clean();
}

void Ghost::update() {

    sprite->update();

    switch (state) {
        case (GHOST_STATE::GS_THROWN):
            tween.step(1);
            dy = tween.peek();
            angle -= 1.0f * facing;
            if (tween.progress() >= 1.0f) {
                dy = 0;
                angle = 0.0f;
                state = GHOST_STATE::GS_IDLE;
                sprite->setAnim("idle");
            }
            break;
        case (GHOST_STATE::GS_GRABBED):
            x += (player->getX() - x) / 2;
            y += (player->getY() - y) / 2;
            dy += (-100 - dy) / 2;
            facing = -player->getFacing();
            vx -= vx * traction;
            vy -= vy * traction;
            break;
        case (GHOST_STATE::GS_DIE):
            // printf("Ghost step... %lf\n", tween.peek());
            // dx = std::sin(TICK / 10) * 8;
            // sprite->setScaleX(tween.peek());
            // sprite->setScaleY(std::abs(1 - tween.peek()));
            sprite->setAlpha(tween.peek() * 100);
            tween.step(1);

            if (tween.progress() == 1.0f) {
                clean_flag = true;
            }
            
            vx -= vx * traction;
            vy -= vy * traction;
            break;
        default:
            dy = std::sin(TICK / 20) * 8;
            
            double _dx = player->getX() - x;
            double _dy = player->getY() - y;
            double _angle = (_dx == 0) ? (M_PI / 2) : std::atan(std::abs(_dy / _dx));
            if (_dx < 0) _angle = M_PI - _angle;
            if (_dy < 0) _angle *= -1;

            vx += move_speed * std::cos(_angle);
            vy += move_speed * std::sin(_angle);

            facing = (_dx >= 0) ? 1 : -1;

            vx -= vx * traction;
            vy -= vy * traction;

            break;
    }

    x += vx;
    y += vy;
}

void Ghost::render() {
    sprite->render(x + dx, y + dy, NULL, angle, NULL, (facing == 1) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void Ghost::seal() {
    state = GS_DIE;
    sprite->setAnim("die");
    tween = tweeny::from(1.0).to(0.0).during(100).via(tweeny::easing::quinticOut);
}

void Ghost::grab() {
    state = GS_GRABBED;
    vx = 0;
    vy = 0;
    sprite->setAnim("die");
}

void Ghost::thrown(double angle) {
    state = GS_THROWN;
    vx = 20 * std::cos(angle);
    vy = 20 * std::sin(angle);
    tween = tweeny::from(-100.0).to(0.0).during(10).via(tweeny::easing::quinticIn);
}
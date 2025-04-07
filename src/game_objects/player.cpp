#include "ghost.h"
#include "player.h"
#include "seal.h"
#include <cmath>
#include "sprite.h"
#include "../global.h"

Player::Player(int _x, int _y) {
    x = _x;
    y = _y;
    sprite = new Sprite();
    sprite->loadTexture("miko");
    sprite->setAnim("idle");
}
Player::~Player() {
    clearSeals();
    clean();
}

void Player::clean() {
    delete seals;
    delete seal_lines;
    delete input_key_down;
    delete input_key_up;
    delete input_keys;
    delete sprite;
}

void Player::render() {

    int seals_size = seals->size();
    if (seals->size() > 0) {
        int prv_x = -1;
        int prv_y = -1;
        for (int i = 0; i < seals_size; i++) {
            Seal* seal = seals->at(i);
            if (prv_x != -1 && prv_y != -1) {
                seal->drawLine(prv_x, prv_y);
            }
            prv_x = seal->getX();
            prv_y = seal->getY();
        }
        if (seals->size() < seals_max) {
            seals->at(seals_size - 1)->drawLine(x, y);
            seals->at(0)->drawLine(x, y);
        }
        else {
            Seal* seal = seals->at(seals_size - 1);
            seals->at(0)->drawLine(seal->getX(), seal->getY());
        }
    }
    
    sprite->render(x, y, NULL, angle, NULL, (facing == 1) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

}

void Player::update() {
    
    // Update sprite
    sprite->update();

    // Tween update
    tween_object->step();

    // Movement
    move_x = input_key_down->at(K_RIGHT) - input_key_down->at(K_LEFT);
    move_y = input_key_down->at(K_DOWN) - input_key_down->at(K_UP);

    if (state_idle == P_S_I_ROLL) {
        move_x = 0;
        move_y = 0;
    }

    if (state_idle == P_S_I_THROW) {
        angle = tween_object->value();
        if (tween_object->isComplete()) {
            state_idle = P_S_I_IDLE;
            angle = 0;
        }
    }

    int _angle = 0;
    if (move_x != 0 && move_y == 0) _angle = 90 - move_x * 90;
    if (move_x == 0 && move_y != 0) _angle = 180 - move_y * 90;
    if (move_x != 0 && move_y != 0) _angle = move_y * (90 - move_x * 45);

    if (move_x != 0 || move_y != 0) {
        if (state_idle == P_S_I_IDLE) {
            state_idle = P_S_I_WALK;
            sprite->setAnim("walk");
        }
    }
    if (state_idle == P_S_I_WALK && move_x == 0 && move_y == 0) {
        state_idle = P_S_I_IDLE;
        sprite->setAnim("idle");
    }
    if (state_idle == P_S_I_ROLL && sprite->isComplete()) {
        state_idle = P_S_I_IDLE;
        sprite->setAnim("idle");
    }

    xv += std::abs(move_x) * move_speed * std::cos(_angle * 3.14 / 180);
    yv += std::abs(move_y) * move_speed * std::sin(_angle * 3.14 / 180);

    if (move_x != 0) facing = move_x;

    roll_cooldown = std::max(roll_cooldown - 1, 0);

    if (input_key_down->at(K_ROLL) && roll_cooldown == 0 && (move_x != 0 || move_y != 0)) {
        xv = 20 * move_speed * std::cos(_angle * 3.14 / 180);
        yv = 20 * move_speed * std::sin(_angle * 3.14 / 180);
        roll_cooldown = roll_cooldown_max;
        if (state_idle != P_S_I_ROLL) {
            state_idle = P_S_I_ROLL;
            sprite->setAnim("roll");
        }
    }

    x += xv;
    y += yv;

    if (!(sprite->getWidth()/2 <= x && x <= WINDOW_WIDTH - sprite->getWidth()/2)) {
        x = std::min(std::max(int(x), sprite->getWidth()/2), WINDOW_WIDTH - sprite->getWidth()/2);
    }
    if (!(sprite->getHeight() <= y && y <= WINDOW_HEIGHT)) {
        y = std::min(std::max(int(y), sprite->getHeight()), WINDOW_HEIGHT);
    }

    xv -= traction * xv;
    yv -= traction * yv;

    // Set Seal input
    seal_delay = std::max(seal_delay - 1, 0);

    if (input_key_down->at(K_SET) && seal_delay == 0 && seals_count > 0) {
        setSeal(x, y);
        seals_count = std::max(seals_count - 1, 0);
        seal_delay = seal_delay_max;
    }

    if (input_key_down->at(K_CLEAR)) {
        seal_delay = 0;
        seals_count = seals_max;
        clearSeals();
    }

    // Grab input
    grab_delay = std::max(grab_delay - 1, 0);
    if (input_key_down->at(K_GRAB)) {
        if (state_idle == P_S_I_GRAB && grabbing != nullptr && grab_delay == 0) {
            thrown();
            (dynamic_cast<Ghost*>(grabbing))->thrown(_angle * 3.14 / 180);
            state_idle = P_S_I_THROW;
            grabbing = nullptr;
            grab_delay = 20;
        }
        if ((state_idle == P_S_I_WALK || state_idle == P_S_I_IDLE) && grab_delay == 0) grab();
    }

    // Seal input
    if (input_key_down->at(K_SEAL) && seals->size() > 2) {
        for (const GameObjectIndex goi : game_objects) {
            if (goi.type == GOT_Ghost) {
                if (Seal::includes(goi.obj->getX(), goi.obj->getY(), seals)) {
                    (dynamic_cast<Ghost*>(goi.obj))->seal();
                }
            }
        }
        clearSeals();
    }
}

void Player::startSeal() {
    
}

void Player::thrown() {
    tween_object->add(0, -30 * M_PI / 180, 40, TWEEN_TYPES::IN_OUT);
    tween_object->add(-30 * M_PI / 180, 30 * M_PI / 180, 40, TWEEN_TYPES::IN_OUT);
    tween_object->add(30 * M_PI / 180, 0, 40, TWEEN_TYPES::IN_OUT);
}

void Player::grab() {
    for (GameObjectIndex goi : game_objects) {
        if (goi.type == GOT_Ghost) {
            Ghost* go = dynamic_cast<Ghost*>(goi.obj);
            int dx = go->getX() - x;
            int dy = go->getY() - y;
            double d = std::sqrt(dx * dx + dy * dy);
            grab_delay = 20;
            if (d <= grab_range) {
                go->grab();
                grabbing = go;
                state_idle = P_S_I_GRAB;
            }
            break;
        }
    }
}
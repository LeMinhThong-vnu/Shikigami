#include "player.h"
#include "enemy.h"
#include "fox.h"
#include "../global.h"
#include <cmath>

Player::Player(int _x, int _y, Game* _game): GameObject(100, 50, _x, _y, _game) {
    input = new InputComponent();
    sprite = new SpriteComponent();
    sprite->loadTexture("miko");
    sprite->setAnim("idle");
    type = PLAYER;
}

Player::~Player() {
    delete input;
}

void Player::update() {
    
    // Update sprite
    sprite->update();

    // Tween update
    tweens->update();

    // Set
    move_x = input->isDown(K_RIGHT) - input->isDown(K_LEFT);
    move_y = input->isDown(K_DOWN) - input->isDown(K_UP);
    
    // Decreasing cooldowns
    roll_cooldown = std::max(roll_cooldown - 1, 0);
    grab_delay = std::max(grab_delay - 1, 0);
    summon_delay = std::max(summon_delay - 1, 0);
    spirit_cooldown = std::max(spirit_cooldown - 1, 0);

    if (spirit_cooldown == 0) {
        spirit_count = std::min(spirit_count + 1, spirit_count_max);
        if (spirit_count < spirit_count_max) {
            spirit_cooldown = spirit_cooldown_max;
        }
    }

    switch (state) {
        case PLR_STE_IDLE: case PLR_STE_ROLL: case PLR_STE_WALK:
            update_idle();
            break;
        case PLR_STE_GRAB: case PLR_STE_THROW:
            update_grab();
            break;
    }

    GameObject::update();
}

void Player::update_idle() {
    int _angle = 0;
    if (move_x != 0 && move_y == 0) _angle = 90 - move_x * 90;
    if (move_x == 0 && move_y != 0) _angle = 180 + move_y * 90;
    if (move_x != 0 && move_y != 0) _angle = -move_y * (90 - move_x * 45);

    if (state == PLR_STE_ROLL) {
        move_x = 0;
        move_y = 0;
    }

    if (move_x != 0 || move_y != 0) {
        if (state == PLR_STE_IDLE) {
            state = PLR_STE_WALK;
            sprite->setAnim("walk");
            body->setTraction(TRC);
        }
    }

    if (state == PLR_STE_WALK && move_x == 0 && move_y == 0) {
        state = PLR_STE_IDLE;
        sprite->setAnim("walk");
    }

    if (move_x != 0 || move_y != 0) body->addVelocityAngle(move_speed, _angle);

    if (move_x != 0) facing = move_x;

    if (input->isDown(K_ROLL) && roll_cooldown == 0 && (move_x != 0 || move_y != 0)) {
        if (state != PLR_STE_ROLL) {
            body->addVelocityAngle(4 * move_speed, _angle);
            roll_cooldown = roll_cooldown_max;
            state = PLR_STE_ROLL;
            sprite->setAnim("roll");
            body->setTraction(0.1);
            body->setActive(false);
        }
    }
    
    // Grab input
    if (grab_delay == 0 && input->isDown(K_GRAB) && grabbing == nullptr) {
        // printf("grab?\n");
        for (PhysicsComponent* _body : body->getColliding()) {
            // printf("found body\n");
            if (_body->getOwner() != NULL) {
                if (_body->getOwner()->is_grabbable()) {
                    // printf("Type: %d\n", _body->getOwner()->getType());
                    if (grab_check(_body->getOwner())) { break; }
                }
            }
        }
        // printf("end...\n");
        // printf("end grab\n");
        grab_delay = grab_delay_max;
    }

    // Roll complete
    if (state == PLR_STE_ROLL && sprite->isComplete()) {
        state = PLR_STE_IDLE;
        sprite->setAnim("idle");
        body->setActive(true);
    }
    
    // Summon input
    if (input->isDown(K_SUMMON) && state != PLR_STE_GRAB && summon_delay == 0 && spirit_count > 0) {
        summon_delay = summon_delay_max;
        Fox* shiki = new Fox(body->getX(), body->getY(), game);
        obj_buffer.push_back(shiki);
        summons.insert(shiki);
        spirit_count--;
        spirit_cooldown = spirit_cooldown_max;
    }
}

bool Player::grab_check(GameObject* obj) {
    switch (obj->getType()) {
        case (ENEMY): {
            // printf("Is enemy\n");
            Enemy* enm = dynamic_cast<Enemy*>(obj);
            if (enm->getState() == ENM_STE_IDLE || enm->getState() == ENM_STE_ATTACK) {
                enm->grabbed();
                state = PLR_STE_GRAB;
                grabbing = enm;
                return true;
            }
            break;
        }
        case (SHIKIGAMI): {
            // printf("Is shiki\n");
            Shikigami* shk = dynamic_cast<Shikigami*>(obj);
            if (shk->get_state() == SHK_STE_IDLE) {
                shk->grabbed();
                state = PLR_STE_GRAB;
                grabbing = shk;
                return true;
            }
            break;
        }
        default: { return false; }
    }
    return false;
}

void Player::update_grab() {
    // printf("grab_state_start...\n");
    int _angle = 0;
    if (move_x == 0 && move_y == 0) _angle = 90 - facing * 90; 
    if (move_x != 0 && move_y == 0) _angle = 90 - move_x * 90;
    if (move_x == 0 && move_y != 0) _angle = 180 + move_y * 90;
    if (move_x != 0 && move_y != 0) _angle = -move_y * (90 - move_x * 45);

    if (state == PLR_STE_THROW) {
        float __angle = 0;
        // printf("throw_state_start\n");
        TweenObject* tween = tweens->get_tween("throw");
        if (tween == nullptr) {
            TweenObject* _tween = new TweenObject();
            _tween->add(0, -30 * facing, 5, TWEEN_TYPES::IN_OUT, "");
            _tween->add(-30 * facing, 30 * facing, 5, TWEEN_TYPES::IN, "");
            _tween->add(30 * facing, 0, 5, TWEEN_TYPES::IN_OUT, "throw");
            tweens->add_tween("throw", tween);
            tween = tweens->get_tween("throw");
        }

        // printf("throw_state_1\n");
        if (!tween->isComplete()) {
            // printf("throw_state_2\n");
            __angle = tween->value();
            // std::cout << __angle << std::endl;
            // __angle = (std::abs(__angle) / __angle) * std::min(std::abs(__angle), 30.0f);
            sprite->setAngle(__angle);
            sprite->setDifferenceX(__angle / 5);
            if (tween->get_flag() == "throw" && grabbing != nullptr) {
                switch (grabbing->getType()) {
                    case (ENEMY): {
                        Enemy* enm = dynamic_cast<Enemy*>(grabbing);
                        enm->thrown(_angle, 5);
                        break;
                    }
                    case (SHIKIGAMI): {
                        Shikigami* shk = dynamic_cast<Shikigami*>(grabbing);
                        shk->thrown(_angle);
                        break;
                    }
                    default: { break; }
                }
                grabbing = nullptr;
            }
            // printf("throw_state_3\n");
        }
        else {
            sprite->setAngle(0);
            sprite->setDifference(0, 0);
            state = PLR_STE_IDLE;
            grab_delay = 20;
        }
    }

    if (move_x != 0 || move_y != 0) {
        if (state == PLR_STE_IDLE) {
            state = PLR_STE_WALK;
            sprite->setAnim("walk");
            body->setTraction(TRC);
        }
    }

    if (state == PLR_STE_WALK && move_x == 0 && move_y == 0) {
        state = PLR_STE_IDLE;
        sprite->setAnim("walk");
    }

    if (move_x != 0 || move_y != 0) body->addVelocityAngle(move_speed / 2, _angle);

    if (move_x != 0 && state != PLR_STE_THROW) facing = move_x;
    
    // Grab input
    if (grab_delay == 0 && input->isDown(K_GRAB) && state == PLR_STE_GRAB && grabbing != nullptr) {
        grab_delay = grab_delay_max;
        state = PLR_STE_THROW;
        // printf("add_tween_start\n");
        TweenObject* tween = new TweenObject();
        tween->add(0, -30 * facing, 10, TWEEN_TYPES::IN_OUT, "");
        tween->add(-30 * facing, 30 * facing, 5, TWEEN_TYPES::IN, "");
        tween->add(30 * facing, 0, 10, TWEEN_TYPES::IN_OUT, "throw");
        // printf("add_tween_end_1\n");
        tweens->add_tween("throw", tween);
        // printf("add_tween_end_2\n");
    }
    // printf("grab_state_end...\n");
}

void Player::update_throw() {

}

void Player::render() {
    GameObject::render();
}
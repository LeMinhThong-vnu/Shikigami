#include "player.h"
#include "enemy.h"
#include "fox.h"
#include "jizo.h"
#include "../global.h"
#include "ui.h"
#include <cmath>

Player::Player(int _x, int _y, Game* _game): GameObject(100, 50, _x, _y, _game) {
    input = new InputComponent();
    sprite = new SpriteComponent();
    sprite->loadTexture("miko");
    sprite->setAnim("knockdown");
    type = PLAYER;
}

Player::~Player() {
    delete input;
}

void Player::update() {
    
    sprite->update();

    tweens->update();

    move_x = input->isDown(K_RIGHT) - input->isDown(K_LEFT);
    move_y = input->isDown(K_DOWN) - input->isDown(K_UP);
    
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
        case PLR_STE_SUMMON:
            update_summon();
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

    if (move_x == 0 && move_y == 0) {
        if (state == PLR_STE_WALK) {
            state = PLR_STE_IDLE;
            sprite->setAnim("walk");
        }
        else if (state != PLR_STE_ROLL && sprite->isComplete()) {
            sprite->setAnim("idle");
        }
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
    
    if (grab_delay == 0 && input->isDown(K_GRAB) && grabbing == nullptr) {
        for (PhysicsComponent* _body : body->getColliding()) {
            if (_body->getOwner() != NULL) {
                if (_body->getOwner()->is_grabbable()) {
                    if (grab_check(_body->getOwner())) { break; }
                }
            }
        }
        grab_delay = grab_delay_max;
    }

    if (state == PLR_STE_ROLL && sprite->isComplete()) {
        state = PLR_STE_IDLE;
        sprite->setAnim("idle");
        body->setActive(true);
    }
    
    if (input->isDown(K_SUMMON) && state != PLR_STE_GRAB && summon_delay == 0) {
        state = PLR_STE_SUMMON;
        sprite->setAnim("summon_pre");
        summon_delay = summon_delay_max;
        TweenObject* tween = new TweenObject();
        tween->add(175, 255, 50, OUT);
        game->get_ui()->tweens->add_tween("summon_alpha", tween);
    }
}

void Player::update_summon() {
    if (sprite->isComplete()) {
        sprite->setAnim("summon");
    }

    if ((input->isDown(K_LEFT) || input->isDown(K_RIGHT)) && summon_delay > 0) { summon_delay = 2; }

    if (input->isDown(K_LEFT) && summon_delay == 0) {
        summon_index--;
        summon_delay = 2;
        if (summon_index < 0) summon_index = summon_index_max;
    }
    if (input->isDown(K_RIGHT) && summon_delay == 0) {
        summon_index++;
        summon_delay = 2;
        if (summon_index > summon_index_max) summon_index = 0;
    }

    if (input->isDown(K_RECYCLE)) {
        sprite->setAnim("summon_post");
        state = PLR_STE_IDLE;
        return;
    }

    if (input->isDown(K_SUMMON) && state != PLR_STE_GRAB && summon_delay == 0) {
        summon_delay = summon_delay_max;
        bool pass = false;
        switch (summon_index) {
            case 0: {
                if (spirit_count >= 1) {
                    pass = true;
                    Fox* shiki = new Fox(body->getX(), body->getY(), game);
                    obj_buffer.push_back(shiki);
                    summons.insert(shiki);
                    grab_check(shiki);
                    spirit_count -= 1;
                }
                break;
            }
            case 1: {
                if (spirit_count >= 2) {
                    pass = true;
                    Jizo* shiki = new Jizo(body->getX(), body->getY(), game);
                    obj_buffer.push_back(shiki);
                    summons.insert(shiki);
                    grab_check(shiki);
                    spirit_count -= 2;
                }
                break;
            }
        }
        if (pass) {
            spirit_cooldown = spirit_cooldown_max;
            sprite->setAnim("summon_post");
            TweenObject* tween = new TweenObject();
            tween->add(255, 175, 50, OUT);
            game->get_ui()->tweens->add_tween("summon_alpha", tween);
        }
        else {
            TweenObject* tween = new TweenObject();
            tween->add(0, 1, 50, IN);
            game->get_ui()->tweens->add_tween("not_enough_spirit", tween);
        }
    }
}

bool Player::grab_check(GameObject* obj) {
    switch (obj->getType()) {
        case (ENEMY): {
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
    int _angle = 0;
    if (move_x == 0 && move_y == 0) _angle = 90 - facing * 90; 
    if (move_x != 0 && move_y == 0) _angle = 90 - move_x * 90;
    if (move_x == 0 && move_y != 0) _angle = 180 + move_y * 90;
    if (move_x != 0 && move_y != 0) _angle = -move_y * (90 - move_x * 45);

    if (state == PLR_STE_THROW) {
        float __angle = 0;
        TweenObject* tween = tweens->get_tween("throw");
        if (tween == nullptr) {
            TweenObject* _tween = new TweenObject();
            _tween->add(0, -30 * facing, 5, TWEEN_TYPES::IN_OUT, "");
            _tween->add(-30 * facing, 30 * facing, 5, TWEEN_TYPES::IN, "");
            _tween->add(30 * facing, 0, 5, TWEEN_TYPES::IN_OUT, "throw");
            tweens->add_tween("throw", tween);
            tween = tweens->get_tween("throw");
        }

        if (!tween->isComplete()) {
            __angle = tween->value();
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
                        switch (shk->get_shiki_type()) {
                            case FOX:
                                dynamic_cast<Fox*>(shk)->thrown(_angle);
                                break;
                            case JIZO:
                                dynamic_cast<Jizo*>(shk)->thrown(_angle);
                                break;
                        }
                        break;
                    }
                    default: { break; }
                }
                grabbing = nullptr;
            }
        }
        else {
            sprite->setAngle(0);
            sprite->setDifference(0, 0);
            state = PLR_STE_IDLE;
            grab_delay = 20;
        }
    }

    if (sprite->get_key() != "summon_post") {
        if (move_x != 0 || move_y != 0) {
            sprite->setAnim("walk", false);
        }

        if (move_x == 0 && move_y == 0) {
            sprite->setAnim("idle", false);
        }
    }
    else {
        if (sprite->isComplete()) sprite->setAnim("idle");
    }

    if (move_x != 0 || move_y != 0) body->addVelocityAngle(move_speed / 2, _angle);

    if (move_x != 0 && state != PLR_STE_THROW) facing = move_x;
    
    if (input->isDown(K_RECYCLE) && grabbing != nullptr && state == PLR_STE_GRAB) {
        if (grabbing->getType() == SHIKIGAMI) {
            sprite->setAnim("summon_post");
            state = PLR_STE_IDLE;
            spirit_count = std::min(spirit_count + (dynamic_cast<Shikigami*>(grabbing)->get_cost() / 2), spirit_count_max);
            if (spirit_count == spirit_count_max) spirit_cooldown = 0;
            grabbing->setRemove(true);
        }
    }

    if (grab_delay == 0 && input->isDown(K_GRAB) && state == PLR_STE_GRAB && grabbing != nullptr) {
        grab_delay = grab_delay_max;
        state = PLR_STE_THROW;
        TweenObject* tween = new TweenObject();
        tween->add(0, -30 * facing, 10, TWEEN_TYPES::IN_OUT, "");
        tween->add(-30 * facing, 30 * facing, 5, TWEEN_TYPES::IN, "");
        tween->add(30 * facing, 0, 10, TWEEN_TYPES::IN_OUT, "throw");
        tweens->add_tween("throw", tween);
    }
}

void Player::update_throw() {

}

void Player::render() {
    GameObject::render();
}
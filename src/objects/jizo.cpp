#include "jizo.h"
#include "enemy.h"
#include <cmath>

Jizo::Jizo(int _x, int _y, Game* _game) : Shikigami(_x, _y, SHIKIGAMI_TYPES::JIZO, _game) {
    sprite = new SpriteComponent();
    sprite->loadTexture("shikigami_jizo");
    sprite->setAnim("idle");
    life_max = 3;
    life = 3;
    cooldown_max = 50;
    cooldown = cooldown_max;
    damage = 20;
    cost = 2;
}

Jizo::~Jizo() {
    
}

void Jizo::thrown(double _angle) {
    attack();
    if (state != SHK_STE_ATTACK) {
        Shikigami::thrown(_angle);
    }
}

void Jizo::update() {

    sprite->update();

    tweens->update();

    cooldown = std::max(cooldown - 1, 0);
    switch (state) {
        case (SHK_STE_IDLE):
            update_idle();
            break;
        case (SHK_STE_ATTACK):
            update_attack();
            break;
        case (SHK_STE_THROWN):
            update_attack();
            break;
        case (SHK_STE_GRABBED):
            update_grabbed();
            break;
        case (SHK_STE_WAIT):
            update_wait();
            break;
        case SHK_STE_DESPAWN:
            update_despawn();
            break;
        default:
            break;
    }
    Shikigami::update();
}

void Jizo::update_idle() {
    if (sprite->isComplete()) {
        sprite->setAnim("idle");
    }

    if (cooldown == 0) {
        attack();
    }

    Shikigami::update_idle();
}

void Jizo::update_wait() {
    if (cooldown == 0) {
        sprite->setAnim("attack");
        attack();
    }
}

void Jizo::attack() {

    for (DistanceBetween dist_btwn : body->getDistances()) {
        GameObject* go = dist_btwn.body->getOwner();
        if (go->getType() == GAME_OBJECT_TYPES::ENEMY) {
            target = dynamic_cast<Enemy*>(go);
            break;
        }
    }
    if (target == nullptr) return;    
    life--;
    state = SHK_STE_ATTACK;
    cooldown = cooldown_max;
    TweenObject* tween_scale = new TweenObject();
    tween_scale->add(0, -0.4, 30, IN_OUT);
    tween_scale->add(-0.4, 0.6, 10, OUT, "jump");
    tween_scale->add(0.6, 0, 30, IN);
    tween_scale->add(0, 0.6, 10, OUT, "fall");
    tween_scale->add(0.6, 0.5, 10, LINEAR);
    tween_scale->add(-0.1, -0.5, 10, OUT, "attack");
    tween_scale->add(-0.5, 0, 30, IN_OUT);
    tweens->add_tween("scale", tween_scale);
    body->setApplyingCollision(false);
}

void Jizo::update_attack() {
    
    TweenObject* tween_scale = tweens->get_tween("scale");
    TweenObject* tween_x = tweens->get_tween("x");
    TweenObject* tween_y = tweens->get_tween("y");
    TweenObject* tween_z = tweens->get_tween("z");

    sprite->setDifferenceX( std::max(std::abs(sprite->getDifferenceX()) - 1, 0.0) * ((Game::get_tick() % 2 == 0) ? 1 : -1) );

    if (tween_scale->get_flag() == "jump") {
        TweenObject* tween_move_x = new TweenObject();
        tween_move_x->add(body->getX(), target->getBody()->getX(), 60, LINEAR);
        TweenObject* tween_move_y = new TweenObject();
        tween_move_y->add(body->getY(), target->getBody()->getY(), 60, LINEAR);
        TweenObject* tween_move_z = new TweenObject();
        tween_move_z->add(body->getZ(), -100, 40, OUT);
        tween_move_z->add(-100, 0, 20, OUT);
        tweens->add_tween("x", tween_move_x);
        tweens->add_tween("y", tween_move_y);
        tweens->add_tween("z", tween_move_z);
        sprite->setAnim("attack");
    }

    if (tween_scale->get_flag() != "") {
        printf("Flag: %s", tween_scale->get_flag().c_str());
    }

    if (tween_scale->get_flag() == "attack") {
        for (DistanceBetween dist_btwn : body->getDistances()) {
            GameObject* go = dist_btwn.body->getOwner();
            if (go->getType() == GAME_OBJECT_TYPES::ENEMY && dist_btwn.distance <= 225) {
                target = dynamic_cast<Enemy*>(go);
                if (target->getState() == ENM_STE_IDLE || target->getState() == ENM_STE_ATTACK) {
                    target->add_damage(50);
                    target->thrown(0, 0);
                }
                Game::set_hitstop(8);
                Game::set_screenshake(8);
            }
        }
        sprite->setDifferenceX(10);
    }

    if (tween_scale->isComplete()) {
        sprite->setAnim("idle");
        state = SHK_STE_IDLE;
    }

    if (tween_scale != nullptr) {
        sprite->setScale(
            1.0 - tween_scale->value(),
            1.0 + tween_scale->value()
        );
    }
    if (tween_x != nullptr && tween_y != nullptr && tween_z != nullptr) {
        body->setPos(
            tween_x->value(),
            tween_y->value(),
            tween_z->value()
        );
    }
}
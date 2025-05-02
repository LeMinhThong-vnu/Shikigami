#include <cmath>
#include "ghost.h"
#include "baby.h"

Ghost::Ghost(int _x, int _y, Game* _game) : Enemy(_x, _y, ENEMY_TYPES::GHOST, _game) {
    sprite = new SpriteComponent();
    sprite->loadTexture("ghost");
    sprite->setAnim("idle");
}

Ghost::~Ghost() {
    // GameObject::~GameObject();
}

void Ghost::update() {
    // Update sprite
    sprite->update();

    // Tween update
    tweens->update();
    
    // Decreasing cooldowns
    stun = std::max(stun - 1, 0);

    switch (state) {
        case ENM_STE_IDLE:
            update_idle();
            break;
        case ENM_STE_GRABBED:
            update_grabbed();
            break;
        case ENM_STE_THROWN:
            update_thrown();
            break;
        case ENM_STE_KNOCKDOWN:
            update_knockdown();
            break;
        case ENM_STE_DEAD:
            update_dead();
            break;
        case ENM_STE_ATTACK:
            update_attack();
            break;
    }

    GameObject::update();
}

void Ghost::update_idle() {
    sprite->setAngle(0);
    sprite->setDifferenceX(0);
    double _dx = game->get_baby()->getBody()->getX() - body->getX();
    double _dy = game->get_baby()->getBody()->getY() - body->getY();
    double _angle = (body->angleFrom(game->get_baby()->getBody()).angle);

    if (_angle != 90 && _angle != 270) {
        facing = (_dx > 0 ? 1 : -1);
    }

    body->addVelocityAngle(1, _angle);

    cooldown = cooldown_max;

    if (body->getDistance(game->get_baby()->getBody()) <= 150) {
        state = ENM_STE_ATTACK;
    }

}

void Ghost::update_attack() {

    if (tweens->get_tween("attack_move") != nullptr) {
        TweenObject* tween_move = tweens->get_tween("attack_move");
        TweenObject* tween_angle = tweens->get_tween("attack_angle");
        sprite->setDifferenceX(tween_move->value());
        sprite->setAngle(tween_angle->value());
        if (tween_move->get_flag() == "attack") {
            game->get_baby()->damage(5);
        }
        if (tween_move->isComplete()) {
            cooldown = cooldown_max;
        }
    }
    else {
        cooldown = std::max(cooldown - 1, 0);
    }

    if (body->getDistance(game->get_baby()->getBody()) > 180) {
        state = ENM_STE_IDLE;
    }

    if (cooldown == 0) {
        cooldown = cooldown_max;
        TweenObject* tween_move = new TweenObject();
        TweenObject* tween_angle = new TweenObject();
        tween_move->add(0, -20 * facing, 15, TWEEN_TYPES::IN_OUT);
        tween_move->add(-20 * facing, 25 * facing, 10, TWEEN_TYPES::OUT);
        tween_move->add(25 * facing, 0, 20, TWEEN_TYPES::OUT, "attack");
        tween_angle->add(0, -20 * facing, 15, TWEEN_TYPES::IN_OUT);
        tween_angle->add(-20 * facing, 45 * facing, 10, TWEEN_TYPES::OUT);
        tween_angle->add(45 * facing, 0, 20, TWEEN_TYPES::IN_OUT);
        tweens->add_tween("attack_move", tween_move);
        tweens->add_tween("attack_angle", tween_angle);
    }
}
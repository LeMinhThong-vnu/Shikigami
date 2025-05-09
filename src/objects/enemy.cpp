#include "enemy.h"
#include "player.h"
#include <cmath>

Enemy::Enemy(int _x, int _y, ENEMY_TYPES _enemy_type, Game* _game) : GameObject(100, 50, _x, _y, _game) {
    enemy_type = _enemy_type;
    type = GAME_OBJECT_TYPES::ENEMY;
    is_enemy = true;
    grabbable = true;
    TweenObject* tween = new TweenObject();
    tween->add(1, 0, 100, IN_OUT);
    tweens->add_tween("spawn", tween);
    body->setPosZ(-100);
}

Enemy::~Enemy() {
}

void Enemy::add_damage(int damage) {
    hp -= damage;
    game->add_score(50);
}

void Enemy::grabbed() {
    state = ENM_STE_GRABBED;
    sprite->setAnim("grabbed");
    body->setVelocity(0, 0, 0);
    body->setGravity(0);
    body->setActive(false);
    body->setApplyingCollision(false);
}

void Enemy::thrown(double _angle, double _v) {
    state = ENM_STE_THROWN;
    sprite->setAnim("thrown");
    body->setVelocityAngle(_v, _angle);
    body->setVelocityZ(-8);
    body->setPosZ(body->getZ() - 1);
    body->setTraction(0);
    body->setGravity(GRV);
    body->setActive(true);
    body->setApplyingCollision(false);
    sprite->setOrigin(0.5, 0.5);
}

void Enemy::update_attack() {

}

void Enemy::update_spawn() {
    TweenObject* tween = tweens->get_tween("spawn");
    sprite->setScale(1 - tween->value(), 1 + tween->value());
    body->setPosZ(-100 * tween->value());
    if (tween->isComplete()) {
        state = ENM_STE_IDLE;
    }
}

void Enemy::update_grabbed() {
    sprite->setAngle(0);
    sprite->setDifferenceX(0);
    PhysicsComponent* player_body = game->getPlayer()->getBody();
    body->setPos(
        body->getX() + (player_body->getX() - body->getX()) / 2,
        body->getY() + (player_body->getY() - body->getY()) / 2,
        body->getZ() + (-100 - body->getZ()) / 2
    );
    facing = game->getPlayer()->getFacing();
    sprite->setDepth(game->getPlayer()->getSprite()->getDepth() + 5);
}

void Enemy::update_thrown() {
    sprite->setDifferenceX(0);
    sprite->setAngle(sprite->getAngle() + 10 * facing);

    for (PhysicsComponent* _body : body->getColliding()) {
        if (_body->getOwner() != NULL) {
            if (_body->getOwner()->isEnemy()) {
                Enemy* go = dynamic_cast<Enemy*>(_body->getOwner());
                if (go->getState() == ENM_STE_IDLE) {
                    go->thrown(5, 90 - 90 * facing);
                    _body->setPosZ(-1);
                    _body->setVelocity(5 * facing, 0);
                    _body->setVelocityZ(-5);
                }
                break;
            }
        }
    }

    if (body->getZ() >= 0) {
        body->setTraction(0.1);
        sprite->setAnim("knockdown");
        stun = 30;
        state = ENM_STE_KNOCKDOWN;
        sprite->setAngle(0);
        body->setActive(true);
        body->setApplyingCollision(true);
        sprite->setOrigin(0.5, 1);
    }
}

void Enemy::update_knockdown() {
    if (stun == 0) {
        state = ENM_STE_IDLE;
        body->resetForces();
        sprite->setAnim("idle");
        if (hp <= 0) {
            state = ENM_STE_DEAD;
            sprite->setAnim("die");
            TweenObject* tween_scale = new TweenObject();
            tween_scale->add(0.0, 0.8, 15, TWEEN_TYPES::OUT, "");
            tween_scale->add(0.8, -1.0, 15, TWEEN_TYPES::IN_OUT, "");
            tweens->add_tween("die", tween_scale);
        }
    }
}

void Enemy::update_dead() {
    TweenObject* tween = tweens->get_tween("die");
    sprite->setScale(1 + tween->value(), 1 - tween->value());
    sprite->setDifferenceY((tween->value() - 0.8) * 20);
    if (tween->isComplete()) {
        remove = true;
    }
}
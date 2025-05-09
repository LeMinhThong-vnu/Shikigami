#include "fox.h"
#include "enemy.h"

Fox::Fox(int _x, int _y, Game* _game) : Shikigami(_x, _y, SHIKIGAMI_TYPES::FOX, _game) {
    sprite = new SpriteComponent();
    sprite->loadTexture("shikigami_fox");
    sprite->setAnim("idle");
    life_max = 3;
    life = 3;
    cooldown = cooldown_max;
    cost = 1;
    sfx = Mix_LoadWAV("assets/audio/shiki_fox.mp3");
}

Fox::~Fox() {
    
}

void Fox::update() {

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

void Fox::update_idle() {
    if (sprite->isComplete()) {
        sprite->setAnim("idle");
    }

    if (cooldown == 0) {
        for (DistanceBetween dist_btwn : body->getDistances()) {
            GameObject* go = dist_btwn.body->getOwner();
            if (go->getType() == GAME_OBJECT_TYPES::ENEMY) {
                target = dynamic_cast<Enemy*>(go);
                life--;
                state = SHK_STE_WAIT;
                cooldown = 10;
                sprite->setAnim("land");
                break;
            }
        }
    }

    Shikigami::update_idle();
}

void Fox::update_wait() {
    if (cooldown == 0) {
        sprite->setAnim("attack");
        attack();
    }
}

void Fox::attack() {
    double _angle = 90 - 90 * facing;
    double _v = 5;
    AngleBetween angl_btwn = body->angleFrom(target->getBody());
    _angle = angl_btwn.angle + (rand() % 10 - 5);
    _v = body->getDistance(target->getBody()) / 20;
    facing = ((_angle > 90 && _angle < 270) ? -1 : 1);
    body->setVelocityAngle(_v, _angle);
    body->setVelocityZ(-7);
    body->setTraction(0.0);
    body->setPosZ(-1);
    body->setApplyingCollision(false);
    state = SHK_STE_ATTACK;
    Mix_PlayChannel(-1, sfx, 0);
}

void Fox::update_attack() {
    for (PhysicsComponent* _body : body->getColliding()) {
        if (_body->getOwner()->getType() == ENEMY) {
            Enemy* go = dynamic_cast<Enemy*>(_body->getOwner());
            if (go->getState() == ENM_STE_IDLE || go->getState() == ENM_STE_ATTACK) {
                go->add_damage(damage);
                go->thrown(90 + facing * 90, -5);
            }
        }
    }
    if (body->getZ() == 0) {
        state = SHK_STE_IDLE;
        body->setTraction(0.2);
        sprite->setAnim("land");
        body->setApplyingCollision(true);
        cooldown = cooldown_max;
    }
}
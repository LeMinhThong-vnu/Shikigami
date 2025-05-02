#include "shikigami.h"
#include "enemy.h"
#include "player.h"

Shikigami::Shikigami(int _x, int _y, SHIKIGAMI_TYPES _type, Game* _game) : GameObject(100, 50, _x, _y, _game) {
    shiki_type = _type;
    type = GAME_OBJECT_TYPES::SHIKIGAMI;
    grabbable = true;
}

Shikigami::~Shikigami() {
    // for (int i = 0; i < player->getSummons().size(); i++) {
    //     Shikigami* shiki = player->getSummons()[i];
    //     std::cout << shiki << std::endl;
    //     std::cout << this << std::endl << std::endl;
    //     if (shiki == this) {
    //         player->getSummons().erase(player->getSummons().begin() + i);
    //     }
    // }
    game->getPlayer()->summons.erase(this);
    // GameObject::~GameObject();
}

void Shikigami::grabbed() {
    state = SHK_STE_GRABBED;
    sprite->setAnim("grabbed");
    body->setVelocity(0, 0, 0);
    body->setGravity(0);
    body->setActive(false);
    body->setApplyingCollision(false);
}

void Shikigami::thrown(double _angle) {
    state = SHK_STE_THROWN;
    sprite->setAnim("thrown");
    body->setVelocityAngle(10, _angle);
    body->setVelocityZ(-8);
    body->setPosZ(body->getZ() - 1);
    body->setTraction(0);
    body->setGravity(GRV);
    body->setActive(true);
    body->setApplyingCollision(false);
}

void Shikigami::attack() {}

void Shikigami::update() {
    GameObject::update();
}

void Shikigami::update_idle() {
    if (life < 0) {
        state = SHK_STE_DESPAWN;
        sprite->setAnim("idle");
        TweenObject* tween_scale = new TweenObject();
        tween_scale->add(0.0, 0.8, 15, TWEEN_TYPES::OUT, "");
        tween_scale->add(0.8, -1.0, 15, TWEEN_TYPES::IN_OUT, "");
        tweens->add_tween("die", tween_scale);
    }
}

void Shikigami::update_wait() {

}

void Shikigami::update_despawn() {
    TweenObject* tween = tweens->get_tween("die");
    sprite->setScale(1 + tween->value(), 1 - tween->value());
    sprite->setDifferenceY((tween->value() - 0.8) * 20);
    if (tween->isComplete()) {
        remove = true;
    }
}

void Shikigami::update_grabbed() {
    // printf("grabbed...\n");
    body->setPos(
        body->getX() + (game->getPlayer()->body->getX() - body->getX()) / 2,
        body->getY() + (game->getPlayer()->body->getY() - body->getY()) / 2,
        body->getZ() + (-100 - body->getZ()) / 2
    );
    facing = game->getPlayer()->getFacing();
    sprite->setDepth(game->getPlayer()->getSprite()->getDepth() + 5);
}

void Shikigami::update_thrown() {
    
}

void Shikigami::update_attack() {
    
}
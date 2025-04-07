#include "enemy.h"

Enemy::Enemy() {
    
}

Enemy::~Enemy() {
    clean();
}

void Enemy::clean() {
    delete sprite;
    delete player;
}

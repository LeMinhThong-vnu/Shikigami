#include "baby.h"
#include <cmath>
#include "../global.h"
#include "../game.h"

Baby::Baby(Game* _game) : GameObject(100, 50, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, _game) {
    type = BABY;
    sprite = new SpriteComponent();
    sprite->loadTexture("baby");
    sprite->setAnim("damage_0");
    sprite->setDepth(WINDOW_HEIGHT / 2);
}

Baby::~Baby() {

}

void Baby::update() {

    sprite->setDifferenceX( std::max(std::abs(sprite->getDifferenceX()) - 1, 0.0) * ((Game::get_tick() % 2 == 0) ? 1 : -1) );
    step = std::min(step + 1, step_max);

    if (hp <= hp_max * 2 / 3) { sprite->setAnim("damage_1"); }
    if (hp <= hp_max * 1 / 3) { sprite->setAnim("damage_2"); }

    sprite->setScale(
        1 + std::sin(double(Game::get_tick()) / step) * 0.2,
        1 - std::sin(double(Game::get_tick()) / step) * 0.2
    );

    body->setPosZ(15 + std::cos(double(Game::get_tick()) / step) * 15);
}

void Baby::damage(int dmg) {
    sprite->setDifferenceX(10);
    step = 12;
    hp -= dmg;
}
#include "ui.h"
#include "shikigami.h"
#include "../game.h"

UI::UI(Game* _owner) {
    game = _owner;
    sprite = new SpriteComponent();
    sprite->loadTexture("ui");
    sprite->setAnim("spirit");
    sprite->setOrigin(0.5, 0.5);
    sprite->setScale(0.5, 0.5);
}

UI::~UI() {
    delete sprite;
}

void UI::update() {
    
}

void UI::render() {
    sprite->update();

    switch (state) {
        case GME_STE_GAME:
            render_game();
            break;
        case GME_STE_PAUSE:
            render_pause();
            break;
        default:
            break;
    }
}

void UI::render_game() {
    int start_x, start_y, ind;
    int offset_x = 75;
    SDL_Rect clip;
    
    // Spirits count
    start_x = 75;
    start_y = 75 + (128 / 2);
    sprite->setOrigin(0.5, 1);
    sprite->setAnim("spirit");
    // sprite->setAlpha(255);
    for (int i = 0; i < game->player->spirit_count; i++) {
        sprite->setAngle(std::sin(Game::TICK / 10) * 10 + (rand() % 2 - 4));
        sprite->setScale(1 - std::cos(Game::TICK / 10) * 0.15, 1 + std::sin(Game::TICK / 10) * 0.15);
        sprite->render(start_x + offset_x * i + std::sin(Game::TICK / 5) * 4, start_y + std::cos(Game::TICK / 10) * 8);
    }
    if (game->player->spirit_count < game->player->spirit_count_max) {
        clip.x = 0;
        clip.w = 128;
        clip.h = 128 * (game->player->spirit_cooldown_max - game->player->spirit_cooldown)/game->player->spirit_cooldown_max;
        clip.y = 128 - clip.h;
        sprite->setAngle(std::sin(Game::TICK / 10) * 10 + (rand() % 2 - 4));
        sprite->setScale(1 - std::cos(Game::TICK / 10) * 0.15, 1 + std::sin(Game::TICK / 10) * 0.15);
        sprite->render(start_x + offset_x * game->player->spirit_count + std::sin(Game::TICK / 5) * 4, start_y + std::cos(Game::TICK / 10) * 8, SDL_FLIP_NONE, &clip);
    }
    // sprite->setAlpha(225 * (1 - (player->spirit_cooldown / player->spirit_cooldown_max)));
    // sprite->render(start_x + 30 * player->spirit_count, start_y);

    // Summons count
    start_x = 75;
    start_y = 200;
    ind = 0;
    sprite->setAlpha(255);
    sprite->setAngle(0);
    // sprite->setAlpha(255);
    for (Shikigami* shiki : game->player->summons) {
        switch (shiki->get_shiki_type()) {
            case FOX: {
                sprite->setAnim("shiki_icon_fox");
                break;
            }
            default:
                sprite->setAnim("shiki_icon_fox");
                break;
        }
        sprite->render(start_x + offset_x * ind + std::sin(Game::TICK / 20) * 2, start_y + std::cos(Game::TICK / 20) * 3);
        ind++;
    }
}

void UI::render_pause() {

}
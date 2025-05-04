#include "ui.h"
#include "shikigami.h"
#include "../game.h"
#include "../util/draw.h"
#include "../global.h"

UI::UI(Game* _owner) {
    game = _owner;
    sprite = new SpriteComponent();
    tweens = new TweenComponent();
    sprite->loadTexture("ui");
    sprite->setAnim("spirit");
    sprite->setOrigin(0.5, 0.5);
    sprite->setScale(0.5, 0.5);
    txtr_text = load_texture("text");
}

UI::~UI() {
    delete sprite;
}

void UI::update() {
    
}

void UI::render() {
    sprite->update();

    tweens->update();

    switch (game->state) {
        case GME_STE_GAME:
            render_game();
            break;
        case GME_STE_PAUSE:
            render_pause();
            break;
        case GME_STE_TITLE:
            render_title();
            break;
        case GME_STE_START:
            render_start();
            break;
        default:
            break;
    }

    tweens->clean_tweens();
}

void UI::render_start() {
    SDL_Rect pos, clip;

    tweens->update();

    // Circle
    TweenObject* tween = tweens->get_tween("start");
    if (tween->isComplete()) {
        game->state = GME_STE_GAME;
    }
    double cirlce_scale = 128 + (tween->value() / 255) * (WINDOW_WIDTH * 2 / 3);
    pos.w = cirlce_scale;
    pos.h = cirlce_scale;
    pos.x = (WINDOW_WIDTH / 2) - (pos.w / 2);
    pos.y = (WINDOW_HEIGHT * 2 / 3) - (pos.h / 2);
    clip.w = 128;
    clip.h = 128;
    clip.x = 511;
    clip.y = 215;
    SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    pos.x = 0;
    pos.y = 0;
    pos.w = WINDOW_WIDTH;
    pos.h = (WINDOW_HEIGHT * 2 / 3) - (cirlce_scale / 2) + 2;
    SDL_RenderFillRect(renderer, &pos);
    pos.x = 0;
    pos.y = 0;
    pos.w = (WINDOW_WIDTH - cirlce_scale) / 2 + 2;
    pos.h = WINDOW_HEIGHT;
    SDL_RenderFillRect(renderer, &pos);
    pos.x = (WINDOW_WIDTH + cirlce_scale) / 2 - 2;
    pos.y = 0;
    pos.w = (WINDOW_WIDTH - cirlce_scale) / 2 + 2;
    pos.h = WINDOW_HEIGHT;
    SDL_RenderFillRect(renderer, &pos);
    pos.x = 0;
    pos.y = (WINDOW_HEIGHT * 2 / 3) + (cirlce_scale / 2) - 2;
    pos.w = WINDOW_WIDTH;
    pos.h = WINDOW_HEIGHT - pos.y + 2;
    SDL_RenderFillRect(renderer, &pos);

    // "Protect the Dreaming Child"
    pos.w = 629 * (1 + tween->value() / 255);
    pos.h = 105 * (1 - tween->value() / 255);
    pos.x = (WINDOW_WIDTH / 2) - (pos.w / 2);
    pos.y = 300 - (pos.h / 2);
    clip.w = 745;
    clip.h = 71;
    clip.x = 0;
    clip.y = 0;
    SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
}

void UI::render_game() {
    double start_x, start_y, ind;
    double offset_x = 75;
    SDL_Rect clip;
    
    // Spirits count
    start_x = 75;
    start_y = 75 + (128 / 2);
    sprite->setOrigin(0.5, 1);
    sprite->setAnim("spirit");
    // sprite->setAlpha(255);
    sprite->setScale(0.5, 0.5);
    for (int i = 0; i < game->player->spirit_count; i++) {
        double angle = i * 2 * M_PI / game->player->spirit_count;
        start_x = game->player->getBody()->getX() + std::cos((Game::TICK / 10) + angle) * offset_x;
        start_y = game->player->getBody()->getY() + std::sin((Game::TICK / 10) + angle) * offset_x - 64;
        sprite->setAngle(std::sin(Game::TICK / 10) * 10 + (rand() % 2 - 4));
        sprite->setScale(1 - std::cos(Game::TICK / 10) * 0.15, 1 + std::sin(Game::TICK / 10) * 0.15);
        sprite->render(start_x + std::sin(Game::TICK / 5) * 4, start_y + std::cos(Game::TICK / 10) * 8);
    }
    if (game->player->spirit_count < game->player->spirit_count_max) {
        clip.x = 0;
        clip.w = 128;
        clip.h = 128 * (game->player->spirit_cooldown_max - game->player->spirit_cooldown)/game->player->spirit_cooldown_max;
        clip.y = 128 - clip.h;
        start_x = game->player->getBody()->getX();
        start_y = game->player->getBody()->getY() - 64;
        sprite->setAlpha(225 * (1 - (game->player->spirit_cooldown / game->player->spirit_cooldown_max)));
        sprite->setAngle(std::sin(Game::TICK / 10) * 10 + (rand() % 2 - 4));
        sprite->setScale(1 - std::cos(Game::TICK / 10) * 0.15, 1 + std::sin(Game::TICK / 10) * 0.15);
        sprite->render(start_x + std::sin(Game::TICK / 5) * 4, start_y + std::cos(Game::TICK / 10) * 8, SDL_FLIP_NONE, &clip);
    }
    sprite->setAlpha(255);

    if (game->player->state == PLR_STE_SUMMON) {
        sprite->setAngle(0);
        switch (game->player->summon_index) {
            case 0: {
                sprite->setAnim("shiki_icon_fox");
                break;
            }
            case 1: {
                sprite->setAnim("shiki_icon_jizo");
                break;
            }
            default:
                sprite->setAnim("shiki_icon_fox");
                break;
        }
        sprite->setScale(0.75, 0.75);
        sprite->render(
            game->player->getBody()->getX() + std::sin(Game::TICK / 20) * 2, 
            game->player->getBody()->getY() - 120 + std::cos(Game::TICK / 20) * 3
        );
        sprite->setAnim("arrow_right");
        sprite->render(
            game->player->getBody()->getX() + 80, 
            game->player->getBody()->getY() - 120
        );
        sprite->setAnim("arrow_left");
        sprite->render(
            game->player->getBody()->getX() - 80, 
            game->player->getBody()->getY() - 120
        );
    }

    if (tweens->get_tween("not_enough_spirit") != nullptr) {
        // "Press Start"
        TweenObject* tween = tweens->get_tween("not_enough_spirit");
        SDL_Rect pos, clip;
        pos.w = 250 * (1 + tween->value());
        pos.h = 39 * (1 - tween->value());
        pos.x = (WINDOW_WIDTH / 2) - (pos.w / 2);
        pos.y = 300 - (pos.h / 2);
        clip.x = 284;
        clip.y = 176;
        clip.w = 250;
        clip.h = 39;
        SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    }

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
            case JIZO: {
                sprite->setAnim("shiki_icon_jizo");
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

void UI::render_title() {
    SDL_Rect pos, clip;
    
    // Circle
    double cirlce_scale = 128;
    pos.w = cirlce_scale;
    pos.h = cirlce_scale;
    pos.x = (WINDOW_WIDTH / 2) - (pos.w / 2);
    pos.y = (WINDOW_HEIGHT * 2 / 3) - (pos.h / 2);
    clip.w = 128;
    clip.h = 128;
    clip.x = 511;
    clip.y = 215;
    SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    pos.x = 0;
    pos.y = 0;
    pos.w = WINDOW_WIDTH;
    pos.h = (WINDOW_HEIGHT * 2 / 3) - (cirlce_scale / 2);
    SDL_RenderFillRect(renderer, &pos);
    pos.x = 0;
    pos.y = 0;
    pos.w = (WINDOW_WIDTH - cirlce_scale) / 2;
    pos.h = WINDOW_HEIGHT;
    SDL_RenderFillRect(renderer, &pos);
    pos.x = (WINDOW_WIDTH + cirlce_scale) / 2;
    pos.y = 0;
    pos.w = (WINDOW_WIDTH - cirlce_scale) / 2;
    pos.h = WINDOW_HEIGHT;
    SDL_RenderFillRect(renderer, &pos);
    pos.x = 0;
    pos.y = (WINDOW_HEIGHT * 2 / 3) + (cirlce_scale / 2);
    pos.w = WINDOW_WIDTH;
    pos.h = WINDOW_HEIGHT - pos.y;
    SDL_RenderFillRect(renderer, &pos);

    // Title
    pos.w = 629;
    pos.h = 105;
    pos.x = (WINDOW_WIDTH / 2) - (pos.w / 2);
    pos.y = 300 - (pos.h / 2);
    clip.w = 629;
    clip.h = 105;
    clip.x = 0;
    clip.y = 71;
    SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    
    // "Press Start"
    pos.w = 227;
    pos.h = 32;
    pos.x = (WINDOW_WIDTH / 2) - (pos.w / 2);
    pos.y = 300 + 105 - (pos.h / 2);
    clip.w = 227;
    clip.h = 32;
    clip.x = 284;
    clip.y = 215;
    SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    
    // "Instructions"
    pos.w = 284;
    pos.h = 182;
    pos.x = (WINDOW_WIDTH * 1 / 3) - (pos.w / 1);
    pos.y = (WINDOW_WIDTH * 2 / 3) - (pos.h / 1);
    clip.x = 0;
    clip.y = 176;
    clip.w = 284;
    clip.h = 182;
    SDL_RenderCopyEx(renderer, txtr_text, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    
}
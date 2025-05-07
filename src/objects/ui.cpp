#include "ui.h"
#include "shikigami.h"
#include "../game.h"
#include "../util/draw.h"
#include "../global.h"
#include <iostream>
#include <json.h>
#include <fstream>

UI::UI(Game* _owner) {
    game = _owner;
    sprite = new SpriteComponent();
    tweens = new TweenComponent();
    sprite->loadTexture("ui");
    sprite->setAnim("spirit");
    sprite->setOrigin(0.5, 0.5);
    sprite->setScale(0.5, 0.5);
    text_texture = load_texture("text");

    std::ifstream file("assets/images/text.json");
    Json::Value data;
    Json::Reader reader;
    reader.parse(file, data);
    Json::Value frame_data = data["frames"];
    for (Json::Value::ArrayIndex i = 0; i != frame_data.size(); i++) {
        AtlusFrame frame;
        std::string key = frame_data.getMemberNames()[i];
        frame.x = frame_data[key]["x"].asInt();
        frame.y = frame_data[key]["y"].asInt();
        frame.w = frame_data[key]["w"].asInt();
        frame.h = frame_data[key]["h"].asInt();
        text_atlus.insert({key, frame});
        std::cout << key << std::endl;
        printf("%s: {\n\tx: %d,\n\ty: %d,\n\tw: %d,\n\th: %d,\n}\n", key.c_str(), frame.x, frame.y, frame.w, frame.h);
    }
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
        case GME_STE_TUTORIAL:
            render_tutorial();
        default:
            break;
    }

    tweens->clean_tweens();
}

void UI::print_text(std::string key, int x, int y) {
    SDL_Rect pos, clip;
    AtlusFrame frame = text_atlus[key];
    pos.w = frame.w;
    pos.h = frame.h;
    pos.x = x - (pos.w / 2);
    pos.y = y - (pos.h / 2);
    clip.w = frame.w;
    clip.h = frame.h;
    clip.x = frame.x;
    clip.y = frame.y;
    SDL_SetTextureAlphaMod(text_texture, 255);
    SDL_RenderCopyEx(renderer, text_texture, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    // printf("%s: {\n\tx: %d,\n\ty: %d,\n\tw: %d,\n\th: %d,\n}\n", key.c_str(), frame.x, frame.y, frame.w, frame.h);
}

void UI::print_text(std::string key, int x, int y, double sx, double sy) {
    SDL_Rect pos, clip;
    AtlusFrame frame = text_atlus[key];
    pos.w = frame.w * sx;
    pos.h = frame.h * sy;
    pos.x = x - (pos.w / 2);
    pos.y = y - (pos.h / 2);
    clip.w = frame.w;
    clip.h = frame.h;
    clip.x = frame.x;
    clip.y = frame.y;
    SDL_SetTextureAlphaMod(text_texture, 255);
    SDL_RenderCopyEx(renderer, text_texture, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    // printf("%s: {\n\tx: %d,\n\ty: %d,\n\tw: %d,\n\th: %d,\n}\n", key.c_str(), frame.x, frame.y, frame.w, frame.h);
}

void UI::print_text(std::string key, int x, int y, double sx, double sy, double alpha) {
    SDL_Rect pos, clip;
    AtlusFrame frame = text_atlus[key];
    pos.w = frame.w * sx;
    pos.h = frame.h * sy;
    pos.x = x - (pos.w / 2);
    pos.y = y - (pos.h / 2);
    clip.w = frame.w;
    clip.h = frame.h;
    clip.x = frame.x;
    clip.y = frame.y;
    SDL_SetTextureAlphaMod(text_texture, alpha);
    SDL_RenderCopyEx(renderer, text_texture, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    // printf("%s: {\n\tx: %d,\n\ty: %d,\n\tw: %d,\n\th: %d,\n}\n", key.c_str(), frame.x, frame.y, frame.w, frame.h);
}

void UI::print_text(std::string key, int x, int y, double sx, double sy, double alpha, double ox, double oy) {
    SDL_Rect pos, clip;
    AtlusFrame frame = text_atlus[key];
    pos.w = frame.w * sx;
    pos.h = frame.h * sy;
    pos.x = x - (pos.w * ox);
    pos.y = y - (pos.h * oy);
    clip.w = frame.w;
    clip.h = frame.h;
    clip.x = frame.x;
    clip.y = frame.y;
    SDL_SetTextureAlphaMod(text_texture, alpha);
    SDL_RenderCopyEx(renderer, text_texture, &clip, &pos, 0.0, NULL, SDL_FLIP_NONE);
    // printf("%s: {\n\tx: %d,\n\ty: %d,\n\tw: %d,\n\th: %d,\n}\n", key.c_str(), frame.x, frame.y, frame.w, frame.h);
}

void UI::render_tutorial() {
    switch (game->player->state) {
        case PLR_STE_IDLE: case PLR_STE_WALK: case PLR_STE_ROLL:{
            print_text("press_i_to_summon", game->player->body->getX(), game->player->body->getY() + 50);
            print_text("wasd_to_move", game->player->body->getX(), game->player->body->getY() + 100);
            break;
        }
        case PLR_STE_SUMMON: {
            print_text("press_i_to_summon", game->player->body->getX(), game->player->body->getY() + 50);
            print_text("press_p_to_cancel", game->player->body->getX(), game->player->body->getY() + 100);
            break;
        }
        case PLR_STE_GRAB: {
            print_text("press_j_to_throw", game->player->body->getX(), game->player->body->getY() + 50);
            print_text("press_p_to_recycle", game->player->body->getX(), game->player->body->getY() + 100);
            break;
        }
        default: break;
    }
    
    for (GameObject* go : game->objects) {
        if (go->getType() == SHIKIGAMI) {
            Shikigami* shiki = dynamic_cast<Shikigami*>(go);
            if (shiki->get_state() == SHK_STE_IDLE) {
                print_text("press_j_to_grab", shiki->getBody()->getX(), shiki->getBody()->getY() + 50);
            }
        }
    }

    render_game();
}

void UI::render_start() {
    TweenObject* tween = tweens->get_tween("tutorial");
    if (tween->isComplete()) {
        game->state = GME_STE_GAME;
    }

    // "Protect the Dreaming Child"
    print_text("protect_the_child", WINDOW_WIDTH / 2, 300, 1 + tween->value() / 255, 1 - tween->value() / 255);
}

void UI::render_game() {
    double start_x, start_y, ind;
    double offset_x = 75;
    SDL_Rect clip, pos;
    double base_alpha = 255;
    if (tweens->get_tween("summon_alpha") != nullptr) {
        base_alpha = tweens->get_tween("summon_alpha")->value();
    }
    else if (game->player->state != PLR_STE_SUMMON) base_alpha = 175;

    // Spirits count
    start_x = 75 + 130;
    start_y = 75 + (86 / 3);
    offset_x = 50;
    print_text("gameplay", 100, 75);
    sprite->setScale(0.5, 0.5);
    sprite->setOrigin(0.5, 1);
    sprite->setAnim("spirit");
    sprite->setAlpha(base_alpha);
    for (int i = 0; i < game->player->spirit_count; i++) {
        sprite->setAngle(std::sin(Game::TICK / 10) * 10 + (rand() % 2 - 4));
        sprite->setScale(1 - std::cos(Game::TICK / 10) * 0.15, 1 + std::sin(Game::TICK / 10) * 0.15);
        sprite->render(start_x + offset_x * i + std::sin(Game::TICK / 5) * 4, start_y + std::cos(Game::TICK / 10) * 8, SDL_FLIP_NONE);
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

    start_x = 75;
    start_y = 75 + (86 * 2 / 3);
    sprite->setAlpha(base_alpha);
    if (game->player->state == PLR_STE_SUMMON) {
        sprite->setAngle(0);
        switch (game->player->summon_index) {
            case 0: {
                sprite->setAnim("shiki_icon_fox_cost");
                break;
            }
            case 1: {
                sprite->setAnim("shiki_icon_jizo_cost");
                break;
            }
            default:
                sprite->setAnim("shiki_icon_fox_cost");
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
        print_text("not_enough_spirit", WINDOW_WIDTH / 2, 300, 1 + tween->value() / 255, 1 - tween->value() / 255);
    }

    if (tweens->get_tween("protect_the_child") != nullptr) {
        // "Press Start"
        TweenObject* tween = tweens->get_tween("protect_the_child");
        print_text("protect_the_child", WINDOW_WIDTH / 2, 300, 1 + tween->value(), 1 - tween->value());
    }

    // Summons count
    start_x = 75 + 130;
    start_y = 75 + (86 * 2 / 3);
    sprite->setAlpha(base_alpha);
    ind = 0;
    sprite->setAngle(0);
    sprite->setScale(0.5, 0.5);
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

    // Title
    print_text("shikigami", WINDOW_WIDTH / 2, 300);
    
    // "Press Start"
    print_text("press_any_key_to_start", WINDOW_WIDTH / 2, 300 + 105);
    
}


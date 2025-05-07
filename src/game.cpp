#include <cmath>
#include "game.h"
#include "util/draw.h"
#include "./objects/game_object.h"
#include "./objects/ui.h"
#include "./objects/shikigami.h"
#include "./objects/enemy.h"
#include "./objects/player.h"
#include "./objects/baby.h"
#include "./objects/ghost.h"
#include "./objects/fox.h"
#include "./objects/jizo.h"

int Game::TICK = 0;
int Game::HITSTOP = 0;
int Game::SCREENSHAKE = 0;

std::vector<GameObject*> Game::objects = {};

Game::Game() {
    layers.insert({ "ui_1", {} });
    layers.insert({ "ui_2", {} });
    layers.insert({ "ui_3", {} });
    layers.insert({ "go_1", {} });
    layers.insert({ "go_2", {} });
    layers.insert({ "go_3", {} });
    tweens = new TweenComponent();
}

Game::~Game() {
    free();
}

void Game::free() {
    delete ui;
    if( shadow != NULL )
	{
		SDL_DestroyTexture( shadow );
	}
    if( background != NULL )
	{
		SDL_DestroyTexture( background );
	}
    for (auto pair : texture_assets) {
        SDL_DestroyTexture(pair.second);
    }
    for (GameObject* goi : Game::objects) {
        remove_object(goi);
    }
    Game::objects.clear();
    layers.clear();
    clean();
}

void Game::setPlayer(Player* _player) {
    player = _player;
    addObject(player);
    ui = new UI(this);
}

void Game::set_baby(Baby* _baby) {
    baby = _baby;
    addObject(baby);
}

Baby* Game::get_baby() { return baby; }

Player* Game::getPlayer() { return player; }

void Game::handleInput() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_EventType::SDL_KEYDOWN:
            if (state == GME_STE_TITLE) {
                state = GME_STE_TUTORIAL;
                player->getSprite()->setAnim("start");
                player->summon_delay = 20;
                TweenObject* tween = new TweenObject();
                tween->add(0, 255, 100, TWEEN_TYPES::IN_OUT);
                ui->tweens->add_tween("start", tween);
                tween = new TweenObject();
                tween->add(255, 0, 100, TWEEN_TYPES::IN_OUT);
                tweens->add_tween("bg_fade_black", tween);
            }
            break;
    }
    player->getInput()->handleInput(event);
}

void Game::debug() {
    
}

void Game::preload() {

}

bool Game::init() {
    // Init Subsys
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "--- SUBSYS INIT ERROR ---" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        std::cout << "-------------------------" << std::endl;
        return false;
    }
    // Create Window
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "--- WINDOW INIT ERROR ---" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        std::cout << "-------------------------" << std::endl;
        return false;
    }
    std::cout << "Created Window!" << std::endl;
    // Create Renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::cout << "--- RENDERER INIT ERROR ---" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        std::cout << "---------------------------" << std::endl;
        return false;
    }
    std::cout << "Created Renderer!" << std::endl;
    // Initiate Fonts
    if (TTF_Init() < 0) {
        std::cout << "--- TTF INIT ERROR ---" << std::endl;
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
        std::cout << "-----------------------" << std::endl;
        return false;
    }
    std::cout << "Initiated TTF!" << std::endl;
    SDL_SetRenderDrawColor(renderer, BG_R, BG_G, BG_B, 1);

    shadow = loadNewTexture("shadow");
    background = loadNewTexture("bg_02");
    SDL_SetTextureAlphaMod(shadow, 50);

    return true;
}

void Game::addObject(GameObject* go) {
    Game::Game::objects.insert(Game::objects.begin(), go);
}

void Game::update() {
    Game::TICK++;

    Game::HITSTOP = std::max(Game::HITSTOP - 1, 0);
    
    Game::SCREENSHAKE = std::max(Game::SCREENSHAKE - 1, 0);

    tweens->update();

    if (Game::HITSTOP == 0) {
        apply_collision();
        
        switch (state) {
            case GME_STE_GAME:
                update_game();
                break;
            case GME_STE_TITLE:
                update_title();
                break;
            case GME_STE_GAMEOVER:
                update_gameover();
                break;
            case GME_STE_PAUSE:
                update_pause();
                break;
            case GME_STE_START:
                update_start();
                break;
            case GME_STE_TUTORIAL:
                update_tutorial();
                break;
        }
    }
}

void Game::update_tutorial() {
    
    if (player->grabbing != nullptr) { 
        if (tutorial_throw_flag) printf("throw_flag\n");
        tutorial_throw_flag = true; 
    }
    if (player->state == PLR_STE_SUMMON) { 
        if (tutorial_summon_flag) printf("summon_flag\n");
        tutorial_summon_flag = true; 
    }

    if (tutorial_summon_flag) {
        if (player->state == PLR_STE_GRAB && player->input->isDown(K_RECYCLE)) {
            if (tutorial_recycle_flag) printf("recycle_flag\n");
            tutorial_recycle_flag = true;
        }
    }

    if (tutorial_recycle_flag && tutorial_summon_flag && tutorial_throw_flag) {
        TweenObject* tween = new TweenObject();
        tween->add(0, 1, 100, IN_OUT);
        baby->get_tweens()->add_tween("tutorial", tween);
        tween = new TweenObject();
        tween->add(0, 1, 100, IN_OUT);
        ui->tweens->add_tween("tutorial", tween);
    }
    
    update_game();
}

void Game::update_start() {
    player->getSprite()->update();
    baby->update();
    if (player->getSprite()->isComplete()) {
        player->getSprite()->setAnim("idle");
    }
}

void Game::update_gameover() {

}

void Game::update_pause() {

}

void Game::update_title() {
    baby->update();
}

void Game::update_game() {

    for (GameObject* goi : Game::objects) {
        switch (goi->getType()) {
            case (GAME_OBJECT_TYPES::PLAYER):
                (dynamic_cast<Player*>(goi))->update();
                break;
            case (GAME_OBJECT_TYPES::BABY):
                (dynamic_cast<Baby*>(goi))->update();
                break;
            case (GAME_OBJECT_TYPES::ENEMY):
                updateEnemy(dynamic_cast<Enemy*>(goi));
                break;
            case (GAME_OBJECT_TYPES::SHIKIGAMI):
                updateShikigami(dynamic_cast<Shikigami*>(goi));
                break;
            default:
                break;
        }
        for (GameObject* go : goi->getObjBuffer()) {
            obj_buffer.push_back(go);
        }
        goi->clearObjBuffer();
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_Texture* screen_tex;
    SDL_Rect screen_pos = {
        (SCREENSHAKE + (SCREENSHAKE != 0 ? rand() % 5 : 0)) * (TICK % 2 == 0 ? 1 : -1) ,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    };
    SDL_SetRenderTarget(renderer, screen_tex);

    render_background();

    for (GameObject* goi : Game::objects) {
        SDL_Rect pos = { 
            int(goi->getBody()->getX() - 64),
            int(goi->getBody()->getY() - 32),
            128,
            64
        };
        SDL_RenderCopy(renderer, shadow, NULL, &pos);
        insertRenderBuffer(goi, "go_2");
    }

    for (const std::pair<std::string, std::vector<GameObject*>> pair : layers) {
        const std::vector<GameObject*> layer = pair.second;
        for (GameObject* goi : layer) {
            switch (goi->getType()) {
                case (GAME_OBJECT_TYPES::PLAYER):
                    (dynamic_cast<Player*>(goi))->render();
                    break;
                case (GAME_OBJECT_TYPES::BABY):
                    (dynamic_cast<Baby*>(goi))->render();
                    break;
                case (GAME_OBJECT_TYPES::ENEMY):
                    renderEnemy(dynamic_cast<Enemy*>(goi));
                    break;
                case (GAME_OBJECT_TYPES::SHIKIGAMI):
                    renderShikigami(dynamic_cast<Shikigami*>(goi));
                    break;
                default:
                    break;
            }
            goi->getBody()->clearColliding();
            goi->getBody()->clearDistances();
        }
    }

    ui->render();

    SDL_RenderCopy(renderer, screen_tex, &screen_pos, NULL);

    SDL_RenderPresent(renderer);
}

void Game::render_background() {
    switch (state) {
        case GME_STE_GAME: {
            SDL_Rect bg_pos = { 
                int(-25 - 25 * std::cos(Game::TICK * M_PI / 270)), 
                int(-25 - 25 * std::sin(Game::TICK * M_PI / 270)), 
                int(WINDOW_WIDTH + 50 + 50 * std::cos(Game::TICK * M_PI / 270)), 
                int(WINDOW_HEIGHT + 50 + 50 * std::sin(Game::TICK * M_PI / 270)) 
            };
            SDL_RenderCopy(renderer, background, NULL, &bg_pos);
            break;
        }
        case GME_STE_TITLE:
            SDL_Rect bg_pos;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            bg_pos.x = 0;
            bg_pos.y = 0;
            bg_pos.w = WINDOW_WIDTH;
            bg_pos.h = WINDOW_HEIGHT;
            SDL_RenderFillRect(renderer, &bg_pos);
            break;
        case GME_STE_TUTORIAL: {
            SDL_Rect bg_pos = { 
                int(-25 - 25 * std::cos(Game::TICK * M_PI / 270)), 
                int(-25 - 25 * std::sin(Game::TICK * M_PI / 270)), 
                int(WINDOW_WIDTH + 50 + 50 * std::cos(Game::TICK * M_PI / 270)), 
                int(WINDOW_HEIGHT + 50 + 50 * std::sin(Game::TICK * M_PI / 270)) 
            };
            SDL_RenderCopy(renderer, background, NULL, &bg_pos);
            if (tweens->get_tween("bg_fade_black") != nullptr) {
                std::cout << tweens->get_tween("bg_fade_black")->value() << std::endl;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, tweens->get_tween("bg_fade_black")->value());
                bg_pos.x = 0;
                bg_pos.y = 0;
                bg_pos.w = WINDOW_WIDTH;
                bg_pos.h = WINDOW_HEIGHT;
                SDL_RenderFillRect(renderer, &bg_pos);
            }
            break;
        }
    }
    
}

void Game::updateEnemy(Enemy* obj) {
    switch (obj->getEnemyType()) {
        case (ENEMY_TYPES::GHOST):
            dynamic_cast<Ghost*>(obj)->update();
            break;
        default:
            break;
    }
}

void Game::renderEnemy(Enemy* obj) {
    switch (obj->getEnemyType()) {
        case (ENEMY_TYPES::GHOST):
            dynamic_cast<Ghost*>(obj)->render();
            break;
    }
}

void Game::updateShikigami(Shikigami* obj) {
    switch (obj->get_shiki_type()) {
        case (SHIKIGAMI_TYPES::FOX):
            dynamic_cast<Fox*>(obj)->update();
            break;
        case (SHIKIGAMI_TYPES::JIZO):
            dynamic_cast<Jizo*>(obj)->update();
            break;
        default:
            break;
    }
}

void Game::renderShikigami(Shikigami* obj) {
    switch (obj->get_shiki_type()) {
        case (SHIKIGAMI_TYPES::FOX):
            dynamic_cast<Fox*>(obj)->render();
            break;
        case (SHIKIGAMI_TYPES::JIZO):
            dynamic_cast<Jizo*>(obj)->render();
            break;
        default:
            break;
    }
}

void Game::apply_collision() {
    for (int i = 0; i < Game::objects.size(); i++) {
        GameObject* goi_base = Game::objects[i];
        for (int j = i + 1; j < Game::objects.size(); j++) {
            GameObject* goi_coll = Game::objects[j];
            goi_base->getBody()->isColliding(goi_coll->getBody());
            goi_base->getBody()->setDistances(goi_coll->getBody());
        }
    }
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Game Cleaned of Objects" << std::endl;
}

void Game::insertRenderBuffer(GameObject* goi, std::string layer_key) {
    std::vector<GameObject*> *layer = &layers.at(layer_key);
    layer->clear();
    for (GameObject* base : Game::objects) {
        int depth_base = base->getSprite()->getDepth();
        int ind = 0;
        for (int i = 0; i < layer->size(); i++) {
            GameObject* reff = layer->at(i);
            int depth_reff = reff->getSprite()->getDepth();
            ind = i;
            if (depth_base <= depth_reff) {
                break;
            }
            if (i == layer->size() - 1) {
                ind++;
            }
        }
        layer->insert(layer->begin() + ind, base);
    }
}

void Game::cleanObjects() {
    for (int i = 0; i < Game::objects.size(); i++) {
        GameObject* goi = Game::objects.at(i);
        if (goi->getRemove()) {
            Game::objects.erase(Game::objects.begin() + i);
            remove_object(goi);
            i--;
        }
        goi->get_tweens()->clean_tweens();
    }

    tweens->clean_tweens();
}

void Game::remove_object(GameObject* goi) {
    switch (goi->getType()) {
        case ENEMY: {
            Enemy* enm = dynamic_cast<Enemy*>(goi);
            delete enm;
            break;
        }
        case SHIKIGAMI: {
            Shikigami* shk = dynamic_cast<Shikigami*>(goi);
            delete shk;
            break;
        }
        case BABY: {
            Baby* bby = dynamic_cast<Baby*>(goi);
            delete bby;
            break;
        }
        default: {
            delete goi;
            break;
        }
    }
}

void Game::addObjectBuffer() {
    for (GameObject* go : obj_buffer) {
        Game::addObject(go);
    }
    obj_buffer.clear();
}
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
                state = GME_STE_START;
                player->getSprite()->setAnim("start");
                TweenObject* tween = new TweenObject();
                tween->add(0, 0, 100, TWEEN_TYPES::IN_OUT);
                tween->add(0, 255, 100, TWEEN_TYPES::IN_OUT);
                ui->tweens->add_tween("start", tween);
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

    // printf("Update start!\n");

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
        }
    }
            
    // printf("Update End!\n");
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
        // std::cout << "Type: " << goi->getType() << std::endl;
        switch (goi->getType()) {
            case (GAME_OBJECT_TYPES::PLAYER):
                // printf("Update Player Start!\n");
                (dynamic_cast<Player*>(goi))->update();
                // printf("Update Player End!\n");
                break;
            case (GAME_OBJECT_TYPES::BABY):
                (dynamic_cast<Baby*>(goi))->update();
                break;
            case (GAME_OBJECT_TYPES::ENEMY):
                // printf("Update Enemy Start!\n");
                updateEnemy(dynamic_cast<Enemy*>(goi));
                // printf("Update Enemy End!\n");
                break;
            case (GAME_OBJECT_TYPES::SHIKIGAMI):
                updateShikigami(dynamic_cast<Shikigami*>(goi));
                // printf("Update Shiki End!\n");
                break;
            default:
                break;
        }
        // printf("update_step_done...\n");
        // if (!goi->getObjBuffer().size() > 0) 
        for (GameObject* go : goi->getObjBuffer()) {
            obj_buffer.push_back(go);
        }
        goi->clearObjBuffer();
    }
}

void Game::render() {
    // printf("Render Start!\n");
    SDL_RenderClear(renderer);
    SDL_Texture* screen_tex;
    SDL_Rect screen_pos = {
        (SCREENSHAKE + (SCREENSHAKE != 0 ? rand() % 5 : 0)) * (TICK % 2 == 0 ? 1 : -1) ,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    };
    SDL_SetRenderTarget(renderer, screen_tex);

    // Render
    SDL_Rect bg_pos = { 
        int(-25 - 25 * std::cos(Game::TICK * M_PI / 270)), 
        int(-25 - 25 * std::sin(Game::TICK * M_PI / 270)), 
        int(WINDOW_WIDTH + 50 + 50 * std::cos(Game::TICK * M_PI / 270)), 
        int(WINDOW_HEIGHT + 50 + 50 * std::sin(Game::TICK * M_PI / 270)) 
    };

    SDL_RenderCopy(renderer, background, NULL, &bg_pos);

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
                    // printf("Render Enemy!\n");
                    renderEnemy(dynamic_cast<Enemy*>(goi));
                    break;
                case (GAME_OBJECT_TYPES::SHIKIGAMI):
                    renderShikigami(dynamic_cast<Shikigami*>(goi));
                    // printf("Render Enemy End!\n");
                    break;
                default:
                    break;
            }
            // printf("render_step_done...\n");
            goi->getBody()->clearColliding();
            goi->getBody()->clearDistances();
            // std::string key = (goi.type == PLAYER ? "player" : "enemy");
            // printf("- %s:\n  + depth: %d\n", key.c_str(), goi->getSprite()->getDepth());
        }
    }

    ui->render();

    SDL_RenderCopy(renderer, screen_tex, &screen_pos, NULL);

    SDL_RenderPresent(renderer);
    // printf("Render end...\n");
}

void Game::updateEnemy(Enemy* obj) {
    switch (obj->getEnemyType()) {
        case (ENEMY_TYPES::GHOST):
            // printf("Update Ghost Start!\n");
            dynamic_cast<Ghost*>(obj)->update();
            // printf("Update Ghost End!\n");
            break;
        default:
            break;
    }
}

void Game::renderEnemy(Enemy* obj) {
    switch (obj->getEnemyType()) {
        case (ENEMY_TYPES::GHOST):
            dynamic_cast<Ghost*>(obj)->render();
            // printf("Render Ghost!\n");
            break;
    }
}

void Game::updateShikigami(Shikigami* obj) {
    switch (obj->get_shiki_type()) {
        case (SHIKIGAMI_TYPES::FOX):
            dynamic_cast<Fox*>(obj)->update();
            // printf("Update Fox End!\n");
            break;
        case (SHIKIGAMI_TYPES::JIZO):
            dynamic_cast<Jizo*>(obj)->update();
            // printf("Update Fox End!\n");
            break;
        default:
            // printf("update Shiki pass\n");
            break;
    }
}

void Game::renderShikigami(Shikigami* obj) {
    switch (obj->get_shiki_type()) {
        case (SHIKIGAMI_TYPES::FOX):
            dynamic_cast<Fox*>(obj)->render();
            // printf("Render Fox End!\n");
            break;
        case (SHIKIGAMI_TYPES::JIZO):
            dynamic_cast<Jizo*>(obj)->render();
            // printf("Update Fox End!\n");
            break;
        default:
            // printf("render Shiki pass\n");
            break;
    }
}

void Game::apply_collision() {
    // printf("Applying Collision!\n");
    for (int i = 0; i < Game::objects.size(); i++) {
        // printf("a\n");
        GameObject* goi_base = Game::objects[i];
        for (int j = i + 1; j < Game::objects.size(); j++) {
            // printf("b\n");
            GameObject* goi_coll = Game::objects[j];
            goi_base->getBody()->isColliding(goi_coll->getBody());
            goi_base->getBody()->setDistances(goi_coll->getBody());
            // std::cout << goi_base->getBody()->isColliding(goi_coll->getBody()) << std::endl;
        }
        // printf("c\n");
    }
    // printf("Applying Collision End!\n");
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
            // printf("next: %d - %d\n", depth_base, depth_reff);
            ind = i;
            if (depth_base <= depth_reff) {
                break;
            }
            if (i == layer->size() - 1) {
                ind++;
            }
        }
        // printf("insert at %d\n", ind);
        layer->insert(layer->begin() + ind, base);
    }
}

void Game::cleanObjects() {
    for (int i = 0; i < Game::objects.size(); i++) {
        GameObject* goi = Game::objects.at(i);
        if (goi->getRemove()) {
            // printf("a\n");
            Game::objects.erase(Game::objects.begin() + i);
            // printf("b\n");
            remove_object(goi);
            i--;
            // printf("c\n");
        }
        goi->get_tweens()->clean_tweens();
    }
}

void Game::remove_object(GameObject* goi) {
    switch (goi->getType()) {
        case ENEMY: {
            // printf("b_enemy\n");
            Enemy* enm = dynamic_cast<Enemy*>(goi);
            delete enm;
            break;
        }
        case SHIKIGAMI: {
            // printf("b_shiki\n");
            Shikigami* shk = dynamic_cast<Shikigami*>(goi);
            delete shk;
            break;
        }
        case BABY: {
            // printf("b_shiki\n");
            Baby* bby = dynamic_cast<Baby*>(goi);
            delete bby;
            break;
        }
        default: {
            // printf("b_0\n");
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
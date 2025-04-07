#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <map>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include "game_object.h"
#include "../util/draw.h"
#include "seal.h"

enum PLAYER_INPUT_KEYS {
    K_UP    = 0,
    K_DOWN  = 1,
    K_LEFT  = 2,
    K_RIGHT = 3,
    K_SET   = 4,
    K_CLEAR = 5,
    K_SEAL  = 6,
    K_ROLL  = 7,
    K_GRAB  = 8,
};

enum PLAYER_STATE_IDLE {
    P_S_I_IDLE = 0,
    P_S_I_WALK = 1,
    P_S_I_ROLL = 2,
    P_S_I_GRAB = 3,
    P_S_I_THROW = 3,
};

/*
    7 8 9
    4 5 6
    1 2 3
*/

enum SEAL_LINE_TYPE {
    L_NONE = 5,
    L_LEFT = 4,
    L_RIGHT = 6,
    L_UP = 8,
    L_DOWN = 2,
    L_DIAGONAL_UP_RIGHT = 9,
    L_DIAGONAL_UP_LEFT = 7,
    L_DIAGONAL_DOWN_RIGHT = 3,
    L_DIAGONAL_DOWN_LEFT = 1
};

class Player : public GameObject {
    public:
        Player(int x, int y);
        ~Player();

        void render();

        void update();

        void handleInput(SDL_Event event) {
            switch (event.type) {
                case SDL_KEYUP:
                    handleInputUp(event.key.keysym.sym);
                    break;
                case SDL_KEYDOWN:
                    handleInputDown(event.key.keysym.sym);
                    break;
            }
        }

        void handleInputDown(SDL_Keycode key) {
            if (input_keys->find(key) == input_keys->end()) return;
            input_key_down->at(input_keys->at(key)) = true;
            input_key_up->at(input_keys->at(key)) = false;
        } 

        void handleInputUp(SDL_Keycode key) {
            if (input_keys->find(key) == input_keys->end()) return;
            input_key_down->at(input_keys->at(key)) = false;
            input_key_up->at(input_keys->at(key)) = true;
        }

        int getSealsCount() { return seals_count; }
        void setSealsCount(int n) { seals_count = n; }

        int getSealsMax() { return seals_max; }
        void setSealsMax(int n) { seals_max = n; }

        void setSeal(int _x, int _y) {
            Seal* seal = new Seal(_x, _y);
            if (seals->size() > 0) {
                Seal* prv = seals->at(seals->size() - 1);
                SEAL_LINE_TYPE type = detectSealLine(prv->getX(), prv->getY(), _x, _y);
                std::cout << type << std::endl;
                seal_lines->push_back(type);
            }
            seals->push_back(seal);
            // Seal::includes(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, seals);
        }

        SEAL_LINE_TYPE detectSealLine(int x1, int y1, int x2, int y2) {
            const int dx_margin = 40;
            const int dy_margin = 40;
            int dx = x2 - x1;
            int dy = y2 - y1;
            bool horizontal = (std::abs(dy) < dy_margin && std::abs(dx) > dx_margin);
            bool vertical = (std::abs(dx) < dx_margin && std::abs(dy) > dy_margin);
            int normal_dx = dx / std::abs(dx);
            int normal_dy = dy / std::abs(dy);
            printf("dx: %d\ndy: %d\n", dx, dy);
            printf("horizontal?: %d\nvertical?: %d\n", horizontal, vertical);
            if (vertical && !horizontal) return ((normal_dy == -1) ? L_UP : L_DOWN);
            if (!vertical && horizontal) return ((normal_dx == 1) ? L_RIGHT : L_LEFT);
            if (vertical && horizontal) return L_NONE;
            if (normal_dx == 1 && normal_dy == -1) return L_DIAGONAL_UP_RIGHT;
            if (normal_dx == -1 && normal_dy == -1) return L_DIAGONAL_UP_LEFT;
            if (normal_dx == 1 && normal_dy == 1) return L_DIAGONAL_DOWN_RIGHT;
            if (normal_dx == -1 && normal_dy == 1) return L_DIAGONAL_DOWN_LEFT;
            return L_NONE;
        }

        void clearSeals() {
            for (Seal* p : *seals) delete p;
            seals->clear();
            seal_lines->clear();
        }

        void startSeal();

        void clean();

        void grab();

        void thrown();

    private:
        int roll_cooldown = 0;
        int roll_cooldown_max = 30;
        int seals_count = 5;
        int seal_delay = 0;
        int seal_delay_max = 20;
        bool seal_down = false;
        int seals_max = 5;
        int move_x = 0;
        int move_y = 0;
        double xv = 0;
        double yv = 0;
        double traction = 0.3;
        int move_speed = 3;
        std::map<SDL_Keycode, PLAYER_INPUT_KEYS> *input_keys = new std::map<SDL_Keycode, PLAYER_INPUT_KEYS>({
            {SDLK_w, K_UP},
            {SDLK_a, K_LEFT},
            {SDLK_s, K_DOWN},
            {SDLK_d, K_RIGHT},
            {SDLK_j, K_SET},
            {SDLK_i, K_SEAL},
            {SDLK_o, K_ROLL},
            {SDLK_u, K_CLEAR},
            {SDLK_p, K_GRAB},
        });
        std::map<PLAYER_INPUT_KEYS, bool> *input_key_down = new std::map<PLAYER_INPUT_KEYS, bool>({
            {K_UP, false},
            {K_LEFT, false},
            {K_DOWN, false},
            {K_RIGHT, false},
            {K_SET, false},
            {K_SEAL, false},
            {K_ROLL, false},
            {K_CLEAR, false},
            {K_GRAB, false},
        });
        std::map<PLAYER_INPUT_KEYS, bool> *input_key_up = new std::map<PLAYER_INPUT_KEYS, bool>({
            {K_UP, true},
            {K_LEFT, true},
            {K_DOWN, true},
            {K_RIGHT, true},
            {K_SET, true},
            {K_SEAL, true},
            {K_ROLL, true},
            {K_CLEAR, true},
            {K_GRAB, true},
        });
        std::vector<Seal*> *seals = new std::vector<Seal*>;
        std::vector<SEAL_LINE_TYPE> *seal_lines = new std::vector<SEAL_LINE_TYPE>;
        PLAYER_STATE_IDLE state_idle = P_S_I_IDLE;
        GameObject* grabbing = nullptr;
        double grab_range = 20;
        int grab_delay = 0;
};

#endif
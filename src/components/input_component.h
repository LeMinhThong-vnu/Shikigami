#ifndef INPUT_COMPONENT
#define INPUT_COMPONENT

#include <map>
#include <SDL.h>
#include "../util/enum_def.h"

class InputComponent {
    public:
        InputComponent();
        ~InputComponent();

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

        bool isDown(PLAYER_INPUT_KEYS key) {
            return input_key_down->at(key);
        }

    private:
    
        void handleInputDown(SDL_Keycode key) {
            if (input_keys->find(key) == input_keys->end()) return;
            input_key_down->at(input_keys->at(key)) = true;
        } 

        void handleInputUp(SDL_Keycode key) {
            if (input_keys->find(key) == input_keys->end()) return;
            input_key_down->at(input_keys->at(key)) = false;
        }
        
        std::map<SDL_Keycode, PLAYER_INPUT_KEYS> *input_keys = new std::map<SDL_Keycode, PLAYER_INPUT_KEYS>({
            {SDLK_w, K_UP},
            {SDLK_a, K_LEFT},
            {SDLK_s, K_DOWN},
            {SDLK_d, K_RIGHT},
            {SDLK_j, K_GRAB},
            {SDLK_i, K_SUMMON},
            {SDLK_o, K_ROLL},
            {SDLK_p, K_RECYCLE},
        });
        std::map<PLAYER_INPUT_KEYS, bool> *input_key_down = new std::map<PLAYER_INPUT_KEYS, bool>({
            {K_UP, false},
            {K_LEFT, false},
            {K_DOWN, false},
            {K_RIGHT, false},
            {K_SUMMON, false},
            {K_GRAB, false},
            {K_ROLL, false},
            {K_RECYCLE, false},
        });
};

#endif
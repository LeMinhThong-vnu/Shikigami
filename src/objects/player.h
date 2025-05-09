#ifndef PLAYER_H
#define PLAYER_H

#include "../util/class_def.h"
#include "game_object.h"
#include <set>
#include "../util/enum_def.h"
#include "../components/input_component.h"

class Player : public GameObject {
    public:
        friend class UI;
        friend class Shikigami;
        friend class Game;
        friend class Baby;
        
        Player(int _x, int _y, Game* _game);
        ~Player();

        void update();

        void render();

        void update_idle();

        void update_grab();

        void update_throw();

        void update_summon();

        void update_gameover();

        InputComponent* getInput() const { return input; };

        void set_state(PLAYER_STATES _state) { state = _state; };

    private:

        bool grab_check(GameObject* obj);

        InputComponent* input;
        PLAYER_STATES state = PLR_STE_IDLE;
        double move_speed = 2.5;
        int roll_cooldown = 0;
        int roll_cooldown_max = 20;
        GameObject* grabbing = nullptr;
        int grab_delay = 0;
        int grab_delay_max = 10;
        int summon_delay = 0;
        int summon_delay_max = 30;
        int summon_max = 3;
        int summon_index = 0;
        int summon_index_max = 1;
        int spirit_cooldown = 0;
        int spirit_cooldown_max = 250;
        int spirit_count = 0;
        int spirit_count_max = 3;
        std::set<Shikigami*> summons = {};
};

#endif
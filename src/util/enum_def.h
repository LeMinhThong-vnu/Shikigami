#ifndef ENUM_DEF_H
#define ENUM_DEF_H

enum GAME_OBJECT_TYPES {
    PLAYER    = 0,
    ENEMY     = 1,
    SHIKIGAMI = 2,
    BABY      = 3,
};

enum ENEMY_TYPES {
    GHOST = 0,
};

enum SHIKIGAMI_TYPES {
    FOX = 0,
};

enum TWEEN_TYPES {
    LINEAR = 1,
    IN     = 2,
    OUT    = 3,
    IN_OUT = 4,
};

enum GAME_STATES {
    GME_STE_GAME     = 0,
    GME_STE_PAUSE    = 1,
    GME_STE_GAMEOVER = 2,
    GME_STE_START    = 3,
    GME_STE_SETTINGS = 4,
};

enum SHIKIGAMI_STATES {
    SHK_STE_IDLE    = 0,
    SHK_STE_ATTACK  = 1,
    SHK_STE_GRABBED = 2,
    SHK_STE_THROWN  = 3,
    SHK_STE_DESPAWN = 4,
    SHK_STE_WAIT    = 5,
};


enum PLAYER_INPUT_KEYS {
    K_UP      = 0, // W
    K_DOWN    = 1, // S
    K_LEFT    = 2, // A
    K_RIGHT   = 3, // D
    K_SUMMON  = 4, // I
    K_ROLL    = 5, // O
    K_GRAB    = 6, // J
    K_RECYCLE = 7, // P
};

enum PLAYER_STATES {
    PLR_STE_IDLE  = 0,
    PLR_STE_WALK  = 1,
    PLR_STE_ROLL  = 2,
    PLR_STE_GRAB  = 3,
    PLR_STE_THROW = 4,
};

enum ENEMY_STATES {
    ENM_STE_IDLE        = 0,
    ENM_STE_GRABBED     = 1,
    ENM_STE_THROWN      = 2,
    ENM_STE_KNOCKDOWN   = 3,
    ENM_STE_HIT         = 4,
    ENM_STE_DEAD        = 5,
    ENM_STE_ATTACK      = 6,
};

enum GAME_STATES {
    GAME,
    PAUSE,
    GAMEOVER,
    TITLE,
    START,
};

#endif
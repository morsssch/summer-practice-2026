#pragma once

#include "types.h"

// Структуры-заглушки
struct GlobalState {
};

struct GameState {
    int  currentScreen = 0;
    int  collectedItems = 0;
    Vec2 playerPos;

};

struct ScreenState {
};

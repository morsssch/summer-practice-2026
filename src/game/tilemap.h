#pragma once

#include "room.h"

// Tile IDs (moved from levels/level_data.h)
constexpr uint8_t T_AIR         = 0;
constexpr uint8_t T_GRASS_TOP   = 1;
constexpr uint8_t T_GROUND      = 2;
constexpr uint8_t T_DARK_TOP    = 3;
constexpr uint8_t T_DARK        = 4;
constexpr uint8_t T_BROWN_TOP   = 5;
constexpr uint8_t T_BROWN       = 6;
constexpr uint8_t T_PLATFORM    = 7;

constexpr uint8_t T_ENEMY_LIGHT = 10;
constexpr uint8_t T_ENEMY_HEAVY = 11;
constexpr uint8_t T_ITEM        = 20;
constexpr uint8_t T_EXIT        = 30;
constexpr uint8_t T_PLAYER      = 40;

bool isSolid(const Room& room, float worldX, float worldY);
bool isPlatform(const Room& room, float worldX, float worldY);

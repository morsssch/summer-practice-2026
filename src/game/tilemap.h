#pragma once

#include "room.h"

constexpr uint8_t T_AIR         = 0;
constexpr uint8_t T_GRASS_TOP   = 1;
constexpr uint8_t T_GROUND      = 2;
constexpr uint8_t T_DARK_TOP    = 3;
constexpr uint8_t T_DARK        = 4;
constexpr uint8_t T_BROWN_TOP   = 5;
constexpr uint8_t T_BROWN       = 6;
constexpr uint8_t T_PLATFORM    = 7;
constexpr uint8_t T_TORCH       = 8;

constexpr uint8_t T_ENEMY_LIGHT = 10;
constexpr uint8_t T_ENEMY_HEAVY = 11;
constexpr uint8_t T_ITEM        = 20;
constexpr uint8_t T_EXIT        = 30;
constexpr uint8_t T_PLAYER      = 40;

constexpr uint8_t T_SPIKE      = 50;
constexpr uint8_t T_SPIKE_CEIL = 51;

constexpr uint8_t T_PLATE_0 = 52;
constexpr uint8_t T_PLATE_1 = 53;
constexpr uint8_t T_PLATE_2 = 54;
constexpr uint8_t T_PLATE_3 = 55;
constexpr uint8_t T_DOOR_0  = 56;
constexpr uint8_t T_DOOR_1  = 57;
constexpr uint8_t T_DOOR_2  = 58;
constexpr uint8_t T_DOOR_3  = 59;
constexpr uint8_t T_DOOR_0L = 60;
constexpr uint8_t T_DOOR_1L = 61;
constexpr uint8_t T_DOOR_2L = 62;
constexpr uint8_t T_DOOR_3L = 63;

inline bool isPlate(uint8_t t) { return t >= T_PLATE_0 && t <= T_PLATE_3; }
inline bool isDoor(uint8_t t)  { return t >= T_DOOR_0  && t <= T_DOOR_3L; }
inline int  plateGroup(uint8_t t) { return t - T_PLATE_0; }
inline int  doorGroup(uint8_t t)  { return (t - T_DOOR_0) % 4; }
inline bool doorFlipped(uint8_t t){ return t >= T_DOOR_0L; }

bool isSolid(const Room& room, float worldX, float worldY);
bool isPlatform(const Room& room, float worldX, float worldY);

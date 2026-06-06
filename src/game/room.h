#pragma once
#include <cstdint>

constexpr int MAX_ROOM_W = 80;
constexpr int MAX_ROOM_H = 60;

struct Room {
    uint8_t tiles[MAX_ROOM_H][MAX_ROOM_W];
    int w = 0;
    int h = 0;
};

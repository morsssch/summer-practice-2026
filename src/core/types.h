#pragma once

#include <cstdint>

constexpr float PI2 = 6.28318530f;

constexpr int TILE_SIZE = 16;
constexpr int WINDOW_W  = 480;
constexpr int WINDOW_H  = 272;

struct Vec2 {
    float x = 0.f;
    float y = 0.f;
};

struct Camera {
    float x    = 0.f;
    float y    = 0.f;
    float zoom = 1.5f;
};

constexpr int MAX_ROOM_W = 80;
constexpr int MAX_ROOM_H = 60;

struct Room {
    uint8_t tiles[MAX_ROOM_H][MAX_ROOM_W];
    int w = 0;
    int h = 0;
};

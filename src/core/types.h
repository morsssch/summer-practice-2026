#pragma once

#include <cstdint>

constexpr int TILE_SIZE      = 16;
constexpr int SCREEN_TILES_W = 40;
constexpr int SCREEN_TILES_H = 22;
constexpr int WINDOW_W       = 640;
constexpr int WINDOW_H       = 352;

struct Vec2 {
    float x = 0.f;
    float y = 0.f;
};

struct Rect {
    float x = 0.f;
    float y = 0.f;
    float w = 0.f;
    float h = 0.f;
};

struct Color {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;
};

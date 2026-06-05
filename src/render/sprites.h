#pragma once

struct SpriteFrame { int x, y, w, h; };

const SpriteFrame TILE_GRASS_TOP = { 16,  0, 16, 16 };
const SpriteFrame TILE_GROUND    = { 16, 16, 16, 16 };
const SpriteFrame TILE_DARK_TOP  = { 96,  0, 16, 16 };
const SpriteFrame TILE_DARK      = { 96, 16, 16, 16 };

const SpriteFrame HERO_IDLE[4] = {
    {  0, 0, 16, 16 }, { 16, 0, 16, 16 },
    { 32, 0, 16, 16 }, { 48, 0, 16, 16 },
};
const SpriteFrame HERO_RUN[6] = {
    {  0, 16, 16, 16 }, { 16, 16, 16, 16 }, { 32, 16, 16, 16 },
    { 48, 16, 16, 16 }, { 64, 16, 16, 16 }, { 80, 16, 16, 16 },
};

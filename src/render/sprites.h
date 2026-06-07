#pragma once

struct SpriteFrame
{
    int x, y, w, h;
};

const SpriteFrame TILE_PURPLE_TL = {0, 0, 16, 16};
const SpriteFrame TILE_GRASS_TOP = {16, 0, 16, 16};
const SpriteFrame TILE_PURPLE_TR = {32, 0, 16, 16};
const SpriteFrame TILE_PURPLE_ML = {0, 16, 16, 16};
const SpriteFrame TILE_GROUND = {16, 16, 16, 16};
const SpriteFrame TILE_PURPLE_MR = {32, 16, 16, 16};
const SpriteFrame TILE_PURPLE_BL = {0, 32, 16, 16};
const SpriteFrame TILE_PURPLE_BM = {16, 32, 16, 16};
const SpriteFrame TILE_PURPLE_BR = {32, 32, 16, 16};

const SpriteFrame TILE_DARK_TL = {96, 0, 16, 16};
const SpriteFrame TILE_DARK_TOP = {112, 0, 16, 16};
const SpriteFrame TILE_DARK_TR = {128, 0, 16, 16};
const SpriteFrame TILE_DARK_ML = {96, 16, 16, 16};
const SpriteFrame TILE_DARK = {112, 16, 16, 16};
const SpriteFrame TILE_DARK_MR = {128, 16, 16, 16};
const SpriteFrame TILE_DARK_BL = {96, 32, 16, 16};
const SpriteFrame TILE_DARK_BM = {112, 32, 16, 16};
const SpriteFrame TILE_DARK_BR = {128, 32, 16, 16};

const SpriteFrame TILE_BROWN_TL = {0, 48, 16, 16};
const SpriteFrame TILE_BROWN_TOP = {16, 48, 16, 16};
const SpriteFrame TILE_BROWN_TR = {32, 48, 16, 16};
const SpriteFrame TILE_BROWN_ML = {0, 64, 16, 16};
const SpriteFrame TILE_BROWN = {16, 64, 16, 16};
const SpriteFrame TILE_BROWN_MR = {32, 64, 16, 16};
const SpriteFrame TILE_BROWN_BL = {0, 80, 16, 16};
const SpriteFrame TILE_BROWN_BM = {16, 80, 16, 16};
const SpriteFrame TILE_BROWN_BR = {32, 80, 16, 16};

const SpriteFrame HERO_DEATH[8] = {
    {0, 0, 16, 16},
    {16, 0, 16, 16},
    {32, 0, 16, 16},
    {48, 0, 16, 16},
    {64, 0, 16, 16},
    {80, 0, 16, 16},
    {96, 0, 16, 16},
    {112, 0, 16, 16},
};
const SpriteFrame HERO_RUN[6] = {
    {0, 16, 16, 16},
    {16, 16, 16, 16},
    {32, 16, 16, 16},
    {48, 16, 16, 16},
    {64, 16, 16, 16},
    {80, 16, 16, 16},
};
const SpriteFrame HERO_IDLE[4] = {
    {0, 80, 16, 16},
    {16, 80, 16, 16},
    {32, 80, 16, 16},
    {48, 80, 16, 16},
};
const SpriteFrame HERO_LAND[2] = {
    {0, 160, 16, 16},
    {16, 160, 16, 16},
};
const SpriteFrame HERO_JUMP[3] = {
    {0, 112, 16, 16},
    {16, 112, 16, 16},
    {32, 112, 16, 16},
};
const SpriteFrame HERO_ROLL[3] = {
    {0, 144, 16, 16},
    {16, 144, 16, 16},
    {32, 144, 16, 16},
};
const SpriteFrame HERO_FALL[3] = {
    {0, 96, 16, 16},
    {16, 96, 16, 16},
    {32, 96, 16, 16},
};
const SpriteFrame HERO_JUMP_FX[4] = {
    {0, 208, 16, 16},
    {16, 208, 16, 16},
    {32, 208, 16, 16},
    {48, 208, 16, 16},
};
const SpriteFrame HERO_ATTACK[4] = {
    {0, 64, 32, 16},
    {32, 64, 32, 16},
    {64, 64, 32, 16},
    {96, 64, 32, 16},
};
const SpriteFrame HERO_ATTACK_BODY[4] = {
    {0, 64, 16, 16},
    {32, 64, 16, 16},
    {64, 64, 16, 16},
    {96, 64, 16, 16},
};
const SpriteFrame HERO_ATTACK_FX[4] = {
    {16, 64, 16, 16},
    {48, 64, 16, 16},
    {80, 64, 16, 16},
    {112, 64, 16, 16},
};
const SpriteFrame HERO_HIT[4] = {
    {0, 128, 16, 16},
    {16, 128, 16, 16},
    {32, 128, 16, 16},
    {48, 128, 16, 16},
};

const SpriteFrame MUSHROOM_WALK[8] = {
    {0, 0, 16, 16},
    {16, 0, 16, 16},
    {32, 0, 16, 16},
    {48, 0, 16, 16},
    {64, 0, 16, 16},
    {80, 0, 16, 16},
    {96, 0, 16, 16},
    {112, 0, 16, 16},
};
const SpriteFrame MUSHROOM_ATTACK[6] = {
    {0, 16, 16, 16},
    {16, 16, 16, 16},
    {32, 16, 16, 16},
    {48, 16, 16, 16},
    {64, 16, 16, 16},
    {80, 16, 16, 16},
};
const SpriteFrame MUSHROOM_DEATH[6] = {
    {0, 32, 16, 16},
    {16, 32, 16, 16},
    {32, 32, 16, 16},
    {48, 32, 16, 16},
    {64, 32, 16, 16},
    {80, 32, 16, 16},
};
const SpriteFrame MUSHROOM_HIT[3] = {
    {0, 48, 16, 16},
    {16, 48, 16, 16},
    {32, 48, 16, 16},
};

const SpriteFrame SLIME_JUMP[15] = {
    {0, 0, 16, 32},
    {16, 0, 16, 32},
    {32, 0, 16, 32},
    {48, 0, 16, 32},
    {64, 0, 16, 32},
    {80, 0, 16, 32},
    {96, 0, 16, 32},
    {112, 0, 16, 32},
    {128, 0, 16, 32},
    {144, 0, 16, 32},
    {160, 0, 16, 32},
    {176, 0, 16, 32},
    {192, 0, 16, 32},
    {208, 0, 16, 32},
    {224, 0, 16, 32},
};
const SpriteFrame SLIME_DEATH[6] = {
    {0, 32, 16, 16},
    {16, 32, 16, 16},
    {32, 32, 16, 16},
    {48, 32, 16, 16},
    {64, 32, 16, 16},
    {80, 32, 16, 16},
};
const SpriteFrame SLIME_IDLE[5] = {
    {0, 48, 16, 16},
    {16, 48, 16, 16},
    {32, 48, 16, 16},
    {48, 48, 16, 16},
    {64, 48, 16, 16},
};
const SpriteFrame SLIME_HIT[3] = {
    {0, 64, 16, 16},
    {16, 64, 16, 16},
    {32, 64, 16, 16},
};

const SpriteFrame TORCH_ANIM[12] = {
    {0, 0, 8, 24},
    {8, 0, 8, 24},
    {16, 0, 8, 24},
    {24, 0, 8, 24},
    {32, 0, 8, 24},
    {40, 0, 8, 24},
    {48, 0, 8, 24},
    {56, 0, 8, 24},
    {64, 0, 8, 24},
    {72, 0, 8, 24},
    {80, 0, 8, 24},
    {88, 0, 8, 24},
};

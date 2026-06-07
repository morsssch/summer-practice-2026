#pragma once
#include <cstdint>

struct Player;

constexpr float SPORE_VISIBLE = 1.4f;

struct SporeCloud {
    float x, y;
    float lifetime;
    float age;
    float delay;
    float dmgTimer;
    bool  active = false;

    static constexpr int PUFF_COUNT = 16;
    struct Puff {
        float ox, oy;
        float dx, dy;
        float radius;
        uint8_t alpha;
    } puffs[PUFF_COUNT];
};

constexpr int MAX_SPORES = 8;

struct SporeField {
    SporeCloud clouds[MAX_SPORES];
};

void spawnSporeCloud(SporeField& sf, float cx, float cy);
void updateSpores(SporeField& sf, Player& p, float dt);

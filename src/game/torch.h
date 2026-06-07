#pragma once
#include "core/types.h"

constexpr int MAX_TORCH = 32;

struct TorchSprite {
    float x, y;
    int animFrame = 0;
    float animTimer = 0.f;
    bool active = false;
};

struct TorchField {
    TorchSprite sprites[MAX_TORCH];
    int         count = 0;
    float       time  = 0.f;
};

void initTorches(TorchField& f, const Room& room);
void updateTorches(TorchField& f, float dt);
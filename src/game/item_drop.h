#pragma once
#include "core/types.h"

struct ItemDrop {
    Vec2  pos;
    Vec2  vel;
    int   type     = -1;
    float lifetime = 0.f;
    bool  active   = false;
};

constexpr int MAX_ITEM_DROPS = 32;

struct ItemField {
    ItemDrop drops[MAX_ITEM_DROPS];
};

void spawnItemDrop(ItemField& f, float x, float y, int type);
void updateItemDrops(ItemField& f, float dt);

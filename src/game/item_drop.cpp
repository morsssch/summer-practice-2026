#include "item_drop.h"

void spawnItemDrop(ItemField& f, float x, float y, int type) {
    for (int i = 0; i < MAX_ITEM_DROPS; i++) {
        if (!f.drops[i].active) {
            f.drops[i] = { {x, y}, {0.f, -60.f}, type, 8.f, true };
            return;
        }
    }
}

void updateItemDrops(ItemField& f, float dt) {
    for (int i = 0; i < MAX_ITEM_DROPS; i++) {
        ItemDrop& d = f.drops[i];
        if (!d.active) continue;
        d.lifetime -= dt;
        if (d.lifetime <= 0.f) d.active = false;
    }
}

#include "grass.h"
#include "player.h"
#include "game/room.h"
#include "core/types.h"
#include "game/tilemap.h"
#include <cmath>
#include <algorithm>

void initGrass(GrassField& g, const Room& room) {
    g.count = 0;
    g.time  = 0.f;

    for (int row = 0; row < room.h && g.count < MAX_GRASS; row++) {
        for (int col = 0; col < room.w && g.count < MAX_GRASS; col++) {
            uint8_t t = room.tiles[row][col];
            if (t != T_GRASS_TOP && t != T_PLATFORM) continue;

            int hash = (col * 2347 + row * 1031 + col * row * 7) % 100;

            int nSprites = 0;
            if      (hash >= 25 && hash < 80) nSprites = 1;
            else if (hash >= 80)              nSprites = 2;

            for (int n = 0; n < nSprites && g.count < MAX_GRASS; n++) {
                GrassSprite& s = g.sprites[g.count++];
                s.active = true;

                int xHash = (col * 3371 + row * 1117 + n * 4943) % 16;
                s.rootX     = col * TILE_SIZE + TILE_SIZE * 0.5f + (xHash - 8) * 0.9f;
                s.rootY     = row * TILE_SIZE + 6;

                int variant = (col * 37 + row * 13 + n * 5) % 12;
                s.tileCol   = variant % 6;
                s.tileRow   = variant / 6;
                s.restAngle = ((col * 7 + n * 3) % 7 - 3) * 0.8f;
                s.angle     = s.restAngle;
                s.angleVel  = 0.f;
            }
        }
    }
}

void updateGrass(GrassField& g, const Player& player, float dt) {
    g.time += dt;

    for (int i = 0; i < g.count; i++) {
        GrassSprite& s = g.sprites[i];
        if (!s.active) continue;

        float wind   = std::sin(g.time * 2.5f + s.rootX * 0.07f) * 20.f;
        float spring = -(s.angle - s.restAngle) * 36.f;
        s.angleVel  += (wind + spring) * dt;
        s.angleVel  *= 0.88f;
        s.angle     += s.angleVel * dt;
        s.angle      = std::clamp(s.angle, -45.f, 45.f);

        float cx = player.pos.x + 8.f;
        float cy = player.pos.y + 8.f;
        if (cx > s.rootX - 10.f && cx < s.rootX + 10.f &&
            cy > s.rootY - 16.f && cy < s.rootY + 4.f)
            s.angleVel += player.vel.x * 0.25f;
    }
}

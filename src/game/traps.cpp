#include "traps.h"
#include "tilemap.h"

void initTraps(TrapSystem& t, Room& room) {
    t = TrapSystem{};

    for (int y = 0; y < room.h; y++) {
        for (int x = 0; x < room.w; x++) {
            uint8_t tile = room.tiles[y][x];

            if (tile == T_SPIKE && t.spikeCount < MAX_SPIKE_TILES) {
                SpikeTile& s = t.spikes[t.spikeCount++];
                s.tx = x; s.ty = y; s.ceil = false; s.active = true;
                room.tiles[y][x] = T_AIR;
            }
            if (tile == T_SPIKE_CEIL && t.spikeCount < MAX_SPIKE_TILES) {
                SpikeTile& s = t.spikes[t.spikeCount++];
                s.tx = x; s.ty = y; s.ceil = true; s.active = true;
                room.tiles[y][x] = T_AIR;
            }

            if (isPlate(tile) && t.plateCount < MAX_PLATE_TILES) {
                PlateTile& pl = t.plates[t.plateCount++];
                pl.tx = x; pl.ty = y;
                pl.group = plateGroup(tile);
                pl.active = true;
                room.tiles[y][x] = T_AIR;
            }

            if (isDoor(tile) && t.doorCount < MAX_DOOR_TILES) {
                DoorTile& d = t.doors[t.doorCount++];
                d.tx = x; d.ty = y;
                d.group  = doorGroup(tile);
                d.flipX  = doorFlipped(tile);
                d.active = true;
                for (int dy = 1; dy < DOOR_TILE_H; dy++) {
                    int aboveY = y - dy;
                    if (aboveY >= 0)
                        room.tiles[aboveY][x] = tile;
                }
            }
        }
    }
}

static bool allPlatesPressed(const TrapSystem& t, int group) {
    bool anyPlate = false;
    for (int i = 0; i < t.plateCount; i++) {
        const PlateTile& pl = t.plates[i];
        if (!pl.active || pl.group != group) continue;
        anyPlate = true;
        if (!pl.pressed && !pl.locked) return false;
    }
    return anyPlate;
}

void updateTraps(TrapSystem& t, Room& room, Player& p, float dt) {
    if (!p.isDead && p.iframeTimer <= 0.f) {
        float px0 = p.pos.x + 2.f, px1 = p.pos.x + 14.f;
        float py0 = p.pos.y + 6.f, py1 = p.pos.y + 16.f;
        for (int i = 0; i < t.spikeCount; i++) {
            if (!t.spikes[i].active) continue;
            float sx0 = t.spikes[i].tx * 16.f;
            float sy0 = t.spikes[i].ty * 16.f;
            if (px1 > sx0 && px0 < sx0 + 16.f && py1 > sy0 && py0 < sy0 + 16.f) {
                damagePlayer(p, 1, 0.f);
                break;
            }
        }
    }

    float px0 = p.pos.x + 1.f, px1 = p.pos.x + 15.f;
    float py0 = p.pos.y,       py1 = p.pos.y + 16.f;
    for (int i = 0; i < t.plateCount; i++) {
        PlateTile& pl = t.plates[i];
        if (!pl.active || pl.locked) continue;
        float plx0 = pl.tx * 16.f, plx1 = plx0 + 16.f;
        float ply0 = pl.ty * 16.f, ply1 = ply0 + 16.f;
        pl.pressed = (px1 > plx0 && px0 < plx1 && py1 > ply0 && py0 < ply1);
    }

    for (int i = 0; i < t.doorCount; i++) {
        DoorTile& d = t.doors[i];
        if (!d.active) continue;

        d.justOpened = false;

        if (d.locked) continue;

        bool allPressed = allPlatesPressed(t, d.group);

        if (d.openFrame == 0) {
            d.idleTimer += dt;
            if (d.idleTimer >= DOOR_IDLE_FRAME_TIME) {
                d.idleTimer -= DOOR_IDLE_FRAME_TIME;
                d.idleFrame = (d.idleFrame + 1) % DOOR_IDLE_FRAMES;
            }
        }

        d.openTimer += dt;
        if (d.openTimer >= DOOR_FRAME_TIME) {
            d.openTimer -= DOOR_FRAME_TIME;
            if (allPressed) {
                if (d.openFrame < DOOR_OPEN_FRAMES - 1)
                    d.openFrame++;
                if (d.openFrame == DOOR_OPEN_FRAMES - 1) {
                    d.locked     = true;
                    d.justOpened = true;
                    for (int dy = 0; dy < DOOR_TILE_H; dy++) {
                        int ry = d.ty - (DOOR_TILE_H - 1 - dy);
                        if (ry >= 0 && ry < room.h)
                            room.tiles[ry][d.tx] = T_AIR;
                    }
                    for (int j = 0; j < t.plateCount; j++) {
                        if (t.plates[j].active && t.plates[j].group == d.group)
                            t.plates[j].locked = true;
                    }
                }
            } else {
                if (d.openFrame > 0) d.openFrame--;
            }
        }
    }
}

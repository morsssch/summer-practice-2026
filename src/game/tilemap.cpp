#include "tilemap.h"
#include "core/types.h"

bool isSolid(const Room& room, float worldX, float worldY) {
    int tileX = (int)(worldX / (float)TILE_SIZE);
    int tileY = (int)(worldY / (float)TILE_SIZE);
    if (tileX < 0 || tileX >= room.w || tileY < 0 || tileY >= room.h) return false;
    uint8_t t = room.tiles[tileY][tileX];
    return t == T_GRASS_TOP || t == T_GROUND
        || t == T_DARK_TOP  || t == T_DARK
        || t == T_BROWN_TOP || t == T_BROWN
        || isDoor(t);
}

bool isPlatform(const Room& room, float worldX, float worldY) {
    int tileX = (int)(worldX / (float)TILE_SIZE);
    int tileY = (int)(worldY / (float)TILE_SIZE);
    if (tileX < 0 || tileX >= room.w || tileY < 0 || tileY >= room.h) return false;
    return room.tiles[tileY][tileX] == T_PLATFORM;
}

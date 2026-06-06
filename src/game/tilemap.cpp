#include "tilemap.h"
#include "levels/level_data.h"
#include "render/sprites.h"

void drawTilemap(Renderer& r, const Room& room) {
    for (int y = 0; y < room.h; y++) {
        for (int x = 0; x < room.w; x++) {
            switch (room.tiles[y][x]) {
                case T_GRASS_TOP: drawTile(r, TILE_GRASS_TOP, x, y); break;
                case T_GROUND:    drawTile(r, TILE_GROUND,    x, y); break;
                case T_DARK_TOP:  drawTile(r, TILE_DARK_TOP,  x, y); break;
                case T_DARK:      drawTile(r, TILE_DARK,      x, y); break;
                case T_BROWN_TOP: drawTile(r, TILE_BROWN_TOP, x, y); break;
                case T_BROWN:     drawTile(r, TILE_BROWN,     x, y); break;
                case T_PLATFORM:  drawTile(r, TILE_GRASS_TOP, x, y); break;
            }
        }
    }
}

bool isSolid(const Room& room, float worldX, float worldY) {
    int tileX = (int)(worldX / 16.f);
    int tileY = (int)(worldY / 16.f);
    if (tileX < 0 || tileX >= room.w || tileY < 0 || tileY >= room.h) return false;
    uint8_t t = room.tiles[tileY][tileX];
    return t == T_GRASS_TOP || t == T_GROUND
        || t == T_DARK_TOP  || t == T_DARK
        || t == T_BROWN_TOP || t == T_BROWN;
}

bool isPlatform(const Room& room, float worldX, float worldY) {
    int tileX = (int)(worldX / 16.f);
    int tileY = (int)(worldY / 16.f);
    if (tileX < 0 || tileX >= room.w || tileY < 0 || tileY >= room.h) return false;
    return room.tiles[tileY][tileX] == T_PLATFORM;
}

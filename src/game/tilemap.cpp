#include "tilemap.h"
#include "levels/level_data.h"
#include "render/sprites.h"

void drawTilemap(Renderer& r, const uint8_t tiles[17][30]) {
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 30; x++) {
            switch (tiles[y][x]) {
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

bool isPlatform(const uint8_t tiles[17][30], float worldX, float worldY) {
    int tileX = (int)(worldX / 16.f);
    int tileY = (int)(worldY / 16.f);
    if (tileX < 0 || tileX >= 30 || tileY < 0 || tileY >= 17) return false;
    return tiles[tileY][tileX] == T_PLATFORM;
}

bool isSolid(const uint8_t tiles[17][30], float worldX, float worldY) {
    int tileX = (int)(worldX / 16.f);
    int tileY = (int)(worldY / 16.f);
    if (tileX < 0 || tileX >= 30 || tileY < 0 || tileY >= 17) return false;
    uint8_t t = tiles[tileY][tileX];
    return t == T_GRASS_TOP || t == T_GROUND
        || t == T_DARK_TOP  || t == T_DARK
        || t == T_BROWN_TOP || t == T_BROWN;
}

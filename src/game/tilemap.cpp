#include "tilemap.h"
#include "levels/level_data.h"
#include "render/sprites.h"

void drawTilemap(Renderer& r, const uint8_t tiles[22][40]) {
    for (int y = 0; y < 22; y++) {
        for (int x = 0; x < 40; x++) {
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

#include "renderer.h"
#include "core/types.h"
#include "game/tilemap.h"
#include "sprites.h"

void drawTilemap(Renderer& r, const Room& room) {
    auto spriteFor = [](uint8_t t) -> const SpriteFrame* {
        switch (t) {
            case T_GRASS_TOP: return &TILE_GRASS_TOP;
            case T_GROUND:    return &TILE_GROUND;
            case T_DARK_TOP:  return &TILE_DARK_TOP;
            case T_DARK:      return &TILE_DARK;
            case T_BROWN_TOP: return &TILE_BROWN_TOP;
            case T_BROWN:     return &TILE_BROWN;
            case T_PLATFORM:  return &TILE_GRASS_TOP;
            default: return nullptr;
        }
    };

    for (int y = 0; y < room.h; y++) {
        for (int x = 0; x < room.w; x++) {
            const SpriteFrame* sf = spriteFor(room.tiles[y][x]);
            if (sf) drawTile(r, *sf, x, y);
        }
    }
}

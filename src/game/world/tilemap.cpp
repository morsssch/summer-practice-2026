#include "game/world/tilemap.h"
#include "core/types.h"

namespace {

constexpr uint8_t F_SOLID = 1 << 0;
constexpr uint8_t F_PLATFORM = 1 << 1;

struct TileFlagTable {
  uint8_t flags[256] = {};
  TileFlagTable() {
    const uint8_t solids[] = {
        T_GRASS_TOP, T_GROUND, T_DARK_TOP, T_DARK, T_BROWN_TOP, T_BROWN,
    };
    for (uint8_t t : solids)
      flags[t] |= F_SOLID;
    for (uint8_t t = T_DOOR_0; t <= T_DOOR_3L; t++)
      flags[t] |= F_SOLID;
    flags[T_PLATFORM] |= F_PLATFORM;
  }
};

const TileFlagTable TILE_FLAGS;

bool hasFlag(const Room &room, float worldX, float worldY, uint8_t flag) {
  int tileX = (int)(worldX / (float)TILE_SIZE);
  int tileY = (int)(worldY / (float)TILE_SIZE);
  if (tileX < 0 || tileX >= room.w || tileY < 0 || tileY >= room.h)
    return false;
  return TILE_FLAGS.flags[room.tiles[tileY][tileX]] & flag;
}

}

bool isSolid(const Room &room, float worldX, float worldY) {
  return hasFlag(room, worldX, worldY, F_SOLID);
}

bool isPlatform(const Room &room, float worldX, float worldY) {
  return hasFlag(room, worldX, worldY, F_PLATFORM);
}

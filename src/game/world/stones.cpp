#include "game/world/stones.h"
#include "game/world/tilemap.h"

void initStones(PhysicsField &f, Room &room) {
  f = PhysicsField{};
  for (int y = 0; y < room.h; y++) {
    for (int x = 0; x < room.w; x++) {
      if (room.tiles[y][x] != T_STONE)
        continue;
      if (spawnPhysicsTile(f, x * 16.f, y * 16.f, PHYS_KIND_STONE))
        room.tiles[y][x] = T_AIR;
    }
  }
}

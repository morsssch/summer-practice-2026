#pragma once
#include "game/world/physics_tile.h"

struct Room;

constexpr int PHYS_KIND_STONE = 0;

void initStones(PhysicsField &f, Room &room);

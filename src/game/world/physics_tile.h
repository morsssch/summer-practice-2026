#pragma once
#include "core/types.h"
#include "game/world/room.h"

struct Player;
struct EnemyField;

constexpr int MAX_PHYS_TILES = 16;
constexpr float PHYS_GRAVITY = 900.f;
constexpr float PHYS_PUSH_SPEED = 70.f;
constexpr float PHYS_MAX_FALL = 800.f;
constexpr float PHYS_CRUSH_VEL = 150.f;

struct PhysicsTile {
  Vec2 pos = {};
  Vec2 vel = {};
  int kind = 0;
  bool pushable = true;
  bool onGround = false;
  bool active = false;
};

struct PhysicsField {
  PhysicsTile tiles[MAX_PHYS_TILES];
  int count = 0;
};

PhysicsTile *spawnPhysicsTile(PhysicsField &f, float x, float y, int kind,
                              bool pushable = true);
void updatePhysicsTiles(PhysicsField &f, const Room &room, Player &player,
                        EnemyField &enemies, float dt);
bool physicsSolidAt(const PhysicsField &f, float wx, float wy);

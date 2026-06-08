#pragma once
#include "core/types.h"
#include "game/actor/enemy.h"
#include "game/actor/player.h"
#include "game/world/room.h"

constexpr int ITEM_ORB = 0;
constexpr int ITEM_POTION = 1;

struct ItemDrop {
  Vec2 pos;
  Vec2 vel;
  int type = -1;
  int animFrame = 0;
  float animTimer = 0.f;
  int collectFrame = 0;
  float collectTimer = 0.f;
  float lifetime = 0.f;
  bool collected = false;
  bool active = false;
};

constexpr int MAX_ITEM_DROPS = 128;

struct ItemField {
  ItemDrop drops[MAX_ITEM_DROPS];
};

void spawnItemDrop(ItemField &f, float x, float y, int type, float vx = 0.f,
                   float vy = -80.f);
void spawnDropFor(ItemField &f, const Enemy &e, bool playerNeedsHeal);
void updateItemDrops(ItemField &f, Player &p, const Room &room, float dt);

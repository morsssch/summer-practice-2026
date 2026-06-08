#pragma once

#include "core/types.h"
#include "game/actor/enemy.h"
#include "game/actor/player.h"
#include "game/actor/spore.h"
#include "game/world/grass.h"
#include "game/world/item_drop.h"
#include "game/world/room.h"
#include "game/world/stones.h"
#include "game/world/torch.h"
#include "game/world/traps.h"

constexpr int ARTIFACT_COUNT = 5;
constexpr int ART_GLOW = 0;
constexpr int ART_WIND = 1;
constexpr int ART_MOTH = 2;
constexpr int ART_SPIKE = 3;
constexpr int ART_SPORE = 4;

constexpr int DEATH_ITEMS = 2;
constexpr float DEATH_ITEM_X = 240.f;
constexpr float DEATH_ITEM_Y0 = 97.f;
constexpr float DEATH_ITEM_DY = 26.f;
constexpr float DEATH_ITEM_W = 160.f;
constexpr float DEATH_ITEM_H = 22.f;

struct DeathScreenState {
  int sel = 0;
  bool kb = false;
  bool isHovering = false;
};

struct GameState {
  Player player;
  Camera camera;
  GrassField grass;
  EnemyField enemies;
  SporeField spores;
  ItemField items;
  TorchField torches;
  TrapSystem traps;
  PhysicsField physics;
  Room room;
  float camShakeTimer = 0.f;
  bool artifacts[ARTIFACT_COUNT] = {};
  bool deathScreen = false;
  DeathScreenState deathSS;
};

void initGameState(GameState &s);

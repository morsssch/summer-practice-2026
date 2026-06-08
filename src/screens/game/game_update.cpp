#include "core/app_state.h"
#include "core/input.h"
#include "game/actor/combat.h"
#include "game/actor/enemy.h"
#include "game/state/game_state.h"
#include "game/world/grass.h"
#include "game/world/room.h"
#include "game/world/stones.h"
#include "game/world/torch.h"
#include "game/world/traps.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

static constexpr PlayerAttackProps makeAttackProps(const bool *artifacts) {
  return {artifacts[ART_SPIKE] ? 24.f : 16.f, 80.f};
}

static void updateCamera(Camera &cam, const Player &player, const Room &room,
                         float dt);
static AppState updateDeathScreen(GameState &s, const Input &input);
static void processPlayerPhase(GameState &s, const Input &input, float dt);
static void processWorldPhase(GameState &s, float dt);
static void resolveCombatPhase(GameState &s);
static void processEffectPhase(GameState &s, float dt);

static AppState updateDeathScreen(GameState &s, const Input &input) {
  DeathScreenState &ds = s.deathSS;
  if (input.upPressed || input.downPressed)
    ds.kb = true;
  if (input.mouseMoved)
    ds.kb = false;
  if (input.upPressed)
    ds.sel = (ds.sel - 1 + DEATH_ITEMS) % DEATH_ITEMS;
  if (input.downPressed)
    ds.sel = (ds.sel + 1) % DEATH_ITEMS;

  ds.isHovering = false;
  bool mouseSelect = false;
  for (int i = 0; i < DEATH_ITEMS; i++) {
    float x0 = DEATH_ITEM_X - DEATH_ITEM_W / 2.f;
    float y0 = DEATH_ITEM_Y0 + i * DEATH_ITEM_DY - DEATH_ITEM_H / 2.f;
    if (input.mouseX >= x0 && input.mouseX <= x0 + DEATH_ITEM_W &&
        input.mouseY >= y0 && input.mouseY <= y0 + DEATH_ITEM_H) {
      ds.isHovering = true;
      if (!ds.kb)
        ds.sel = i;
      if (input.mouseClicked) {
        ds.sel = i;
        mouseSelect = true;
      }
    }
  }

  if (input.confirmPressed || mouseSelect) {
    if (ds.sel == 0)
      initGameState(s);
    else
      return AppState::MENU;
  }
  if (input.escapePressed)
    return AppState::MENU;
  return AppState::GAME;
}

static void processPlayerPhase(GameState &s, const Input &input, float dt) {
  s.player.hasDoubleJump = s.artifacts[ART_WIND];
  updatePlayer(s.player, input, s.room, s.physics, dt);
}

static void processWorldPhase(GameState &s, float dt) {
  updateEnemies(s.enemies, s.room, s.physics, s.player.pos, dt);
  updatePhysicsTiles(s.physics, s.room, s.player, s.enemies, dt);
  updateTraps(s.traps, s.room, s.player, s.physics, dt);
}

static void resolveCombatPhase(GameState &s) {
  resolveAttack(s.player, s.enemies, makeAttackProps(s.artifacts));
  resolveEnemyContact(s.enemies, s.player);
  resolveEnemyAttack(s.enemies, s.player);
}

static void processEffectPhase(GameState &s, float dt) {
  for (int i = 0; i < s.traps.doorCount; i++) {
    if (s.traps.doors[i].justOpened) {
      s.camShakeTimer = 0.55f;
      break;
    }
  }
  if (s.camShakeTimer > 0.f)
    s.camShakeTimer -= dt;

  for (int i = 0; i < s.enemies.count; i++) {
    Enemy &e = s.enemies.enemies[i];
    if (!e.active || e.dying || e.anim != EnemyAnim::ATTACK ||
        e.attackFxSpawned)
      continue;
    if (ENEMY_TEMPLATES[e.type].attackEffect == AttackEffect::SPORE_CLOUD) {
      spawnSporeCloud(s.spores, e.pos.x + 8.f, e.pos.y);
    }
    e.attackFxSpawned = true;
  }
  for (int i = 0; i < s.enemies.count; i++) {
    Enemy &e = s.enemies.enemies[i];
    if (!e.active || !e.dying || e.dropSpawned)
      continue;
    e.dropSpawned = true;
    bool playerNeedsHeal = s.player.currentHp < s.player.maxHp;
    spawnDropFor(s.items, e, playerNeedsHeal);
  }

  updateItemDrops(s.items, s.player, s.room, dt);
  updateSpores(s.spores, s.player, dt);
  updateGrass(s.grass, s.player, s.enemies, dt);
  updateTorches(s.torches, dt);
}

AppState updateGameImpl(GameState &s, const Input &input, float dt) {
  if (s.deathScreen)
    return updateDeathScreen(s, input);

  if (input.escapePressed)
    return AppState::PAUSE;
  processPlayerPhase(s, input, dt);
  processWorldPhase(s, dt);
  resolveCombatPhase(s);
  processEffectPhase(s, dt);

  if (s.player.isDead && s.player.deathTimer <= 0.f) {
    s.deathScreen = true;
    s.deathSS = {};
  }

  updateCamera(s.camera, s.player, s.room, dt);
  return AppState::GAME;
}

static void updateCamera(Camera &cam, const Player &player, const Room &room,
                         float dt) {
  float viewW = WINDOW_W / cam.zoom;
  float viewH = WINDOW_H / cam.zoom;

  float targetX = player.pos.x + 8 - viewW / 2.f;
  float targetY = player.pos.y + 8 - viewH / 2.f;

  cam.x += (targetX - cam.x) * (1.f - std::exp(-8.f * dt));
  cam.y += (targetY - cam.y) * (1.f - std::exp(-8.f * dt));

  cam.x = std::clamp(cam.x, 0.f, room.w * TILE_SIZE - viewW);
  cam.y = std::clamp(cam.y, 0.f, room.h * TILE_SIZE - viewH);
}

#include "game/state/game_state.h"
#include "levels/screen_01.h"
#include <algorithm>

void initGameState(GameState &s) {
  s.player = Player{};
  s.player.pos = {80.f, 496.f};
  s.camera = Camera{};
  s.room = SCREEN_01;
  s.deathScreen = false;
  s.deathSS = {};
  s.spores = {};
  s.items = {};
  s.camShakeTimer = 0.f;
  for (int i = 0; i < ARTIFACT_COUNT; i++)
    s.artifacts[i] = false;
  s.artifacts[ART_WIND] = true;
  initEnemies(s.enemies, s.room);

  float viewW = WINDOW_W / s.camera.zoom;
  float viewH = WINDOW_H / s.camera.zoom;
  s.camera.x = std::clamp(s.player.pos.x + 8 - viewW / 2.f, 0.f,
                          s.room.w * TILE_SIZE - viewW);
  s.camera.y = std::clamp(s.player.pos.y + 8 - viewH / 2.f, 0.f,
                          s.room.h * TILE_SIZE - viewH);
  initGrass(s.grass, s.room);
  initTorches(s.torches, s.room);
  initTraps(s.traps, s.room);
  initStones(s.physics, s.room);
}

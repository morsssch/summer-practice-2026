#include "game/actor/spore.h"
#include "core/types.h"
#include "game/actor/player.h"
#include <algorithm>
#include <cmath>

static constexpr float SPORE_HALF_W = 24.f;
static constexpr float SPORE_HALF_H = 14.f;
static constexpr float SPORE_DMG_CD = 0.5f;

void spawnSporeCloud(SporeField &sf, float cx, float cy) {
  for (int i = 0; i < MAX_SPORES; i++) {
    if (!sf.clouds[i].active) {
      SporeCloud &c = sf.clouds[i];
      c = SporeCloud{};
      c.x = cx;
      c.y = cy;
      c.lifetime = SPORE_VISIBLE;
      c.age = 0.f;
      c.delay = 0.35f;
      c.dmgTimer = 0.f;
      c.active = true;

      int seed = (int)(cx) * 7 + (int)(cy) * 13;
      for (int j = 0; j < SporeCloud::PUFF_COUNT; j++) {
        int hAngle = (seed + j * 37 + j * j * 3) % 360;
        float angle = hAngle * (PI2 / 360.f);
        int hDist = (seed * 3 + j * 17) % 4;
        float dist = 2.f + hDist * 6.f;

        c.puffs[j].ox = std::cos(angle) * dist;
        c.puffs[j].oy = std::sin(angle) * dist * 0.55f;
        c.puffs[j].dx = std::cos(angle) * (5.f + hDist * 3.f);
        c.puffs[j].dy = std::sin(angle) * 2.f - 5.f;
        c.puffs[j].radius = 5.f + (j % 5) * 2.f;
        c.puffs[j].alpha = (uint8_t)(55 + (j % 4) * 18);
      }
      return;
    }
  }
}

void updateSpores(SporeField &sf, Player &p, float dt) {
  for (int i = 0; i < MAX_SPORES; i++) {
    SporeCloud &c = sf.clouds[i];
    if (!c.active)
      continue;
    if (c.delay > 0.f) {
      c.delay -= dt;
      continue;
    }

    c.age += dt;
    c.lifetime -= dt;
    if (c.lifetime <= 0.f) {
      c.active = false;
      continue;
    }
    if (c.dmgTimer > 0.f) {
      c.dmgTimer -= dt;
      continue;
    }

    float px = p.pos.x + 8.f;
    float py = p.pos.y + 8.f;
    if (px > c.x - SPORE_HALF_W && px < c.x + SPORE_HALF_W &&
        py > c.y - SPORE_HALF_H && py < c.y + SPORE_HALF_H) {
      damagePlayer(p, 1, p.pos.x < c.x ? -100.f : 100.f);
      c.dmgTimer = SPORE_DMG_CD;
    }
  }
}

#include "game/world/item_drop.h"
#include "game/world/tilemap.h"
#include <algorithm>
#include <cmath>

static constexpr float GRAVITY = 400.f;
static constexpr float ORB_FRAME_TIME = 0.10f;
static constexpr int ORB_FRAMES = 6;
static constexpr float COLL_FRAME_TIME = 0.06f;
static constexpr int COLL_FRAMES = 5;
static constexpr float PICKUP_RADIUS = 14.f;
static constexpr float FLY_SPEED = 180.f;

void spawnItemDrop(ItemField &f, float x, float y, int type, float vx,
                   float vy) {
  for (int i = 0; i < MAX_ITEM_DROPS; i++) {
    if (!f.drops[i].active) {
      ItemDrop &d = f.drops[i];
      d = ItemDrop{};
      d.pos = {x, y};
      d.vel = {vx, vy};
      d.type = type;
      d.lifetime = 12.f;
      d.active = true;
      return;
    }
  }
}

void spawnDropFor(ItemField &f, const Enemy &e, bool playerNeedsHeal) {
  int roll = rand() % 100;
  if (roll < 10 && playerNeedsHeal) {
    spawnItemDrop(f, e.pos.x + 4.f, e.pos.y + 4.f, ITEM_POTION);
  } else if (roll < 90) {
    int r2 = rand() % 100;
    int orbCount = (r2 < 20) ? 3 : (r2 < 55) ? 2 : 1;
    for (int j = 0; j < orbCount; j++) {
      float vx = (float)(rand() % 60) - 30.f;
      float vy = -60.f - (float)(rand() % 50);
      spawnItemDrop(f, e.pos.x + 4.f, e.pos.y + 4.f, ITEM_ORB, vx, vy);
    }
  }
}

void updateItemDrops(ItemField &f, Player &p, const Room &room, float dt) {
  for (int i = 0; i < MAX_ITEM_DROPS; i++) {
    ItemDrop &d = f.drops[i];
    if (!d.active)
      continue;

    if (!d.collected) {
      d.vel.y += GRAVITY * dt;
      d.pos.x += d.vel.x * dt;
      d.pos.y += d.vel.y * dt;

      if (d.vel.y > 0.f && (isSolid(room, d.pos.x + 4.f, d.pos.y + 8.f) ||
                            isPlatform(room, d.pos.x + 4.f, d.pos.y + 8.f))) {
        int tileY = (int)((d.pos.y + 8.f) / TILE_SIZE);
        d.pos.y = (float)(tileY * TILE_SIZE) - 8.f;
        d.vel = {0.f, 0.f};
      }
      if (d.vel.y < 0.f && isSolid(room, d.pos.x + 4.f, d.pos.y)) {
        d.vel.y = 0.f;
      }

      if (d.type == ITEM_ORB) {
        d.animTimer += dt;
        if (d.animTimer >= ORB_FRAME_TIME) {
          d.animTimer -= ORB_FRAME_TIME;
          d.animFrame = (d.animFrame + 1) % ORB_FRAMES;
        }
      }

      d.lifetime -= dt;
      if (d.lifetime <= 0.f) {
        d.active = false;
        continue;
      }

      if (!p.isDead) {
        float cx = d.pos.x + 4.f, cy = d.pos.y + 4.f;
        float px = p.pos.x + 8.f, py = p.pos.y + 8.f;
        float ddx = px - cx, ddy = py - cy;
        bool inRange = ddx * ddx + ddy * ddy < PICKUP_RADIUS * PICKUP_RADIUS;

        if (inRange) {
          if (d.type == ITEM_ORB) {
            p.orbs++;
            d.collected = true;
          } else if (d.type == ITEM_POTION && p.currentHp < p.maxHp) {
            int prevHp = p.currentHp;
            p.currentHp = p.maxHp;
            p.gainedHeartSlot = prevHp;
            p.gainedHeartTimer = 0.f;
            p.gainedHeartFrame = 4;
            p.gainedHeartFrameTimer = 0.f;
            d.collected = true;
          }
        }
      }
    } else {
      float tx = p.pos.x + 8.f, ty = p.pos.y + 8.f;
      float ddx = tx - (d.pos.x + 4.f), ddy = ty - (d.pos.y + 4.f);
      float dist = std::sqrt(ddx * ddx + ddy * ddy);
      if (dist > 3.f) {
        float speed = FLY_SPEED + (30.f - std::min(dist, 30.f)) * 3.f;
        d.pos.x += (ddx / dist) * speed * dt;
        d.pos.y += (ddy / dist) * speed * dt;
      } else {
        d.active = false;
        continue;
      }
      d.collectTimer += dt;
      if (d.collectTimer >= COLL_FRAME_TIME) {
        d.collectTimer -= COLL_FRAME_TIME;
        d.collectFrame = (d.collectFrame + 1) % COLL_FRAMES;
      }
    }
  }
}

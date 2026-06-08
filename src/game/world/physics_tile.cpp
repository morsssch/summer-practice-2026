#include "game/world/physics_tile.h"
#include "game/actor/enemy.h"
#include "game/actor/player.h"
#include "game/world/tilemap.h"
#include <algorithm>
#include <cmath>

PhysicsTile *spawnPhysicsTile(PhysicsField &f, float x, float y, int kind,
                              bool pushable) {
  if (f.count >= MAX_PHYS_TILES)
    return nullptr;
  PhysicsTile &t = f.tiles[f.count++];
  t = PhysicsTile{};
  t.pos = {x, y};
  t.kind = kind;
  t.pushable = pushable;
  t.active = true;
  return &t;
}

bool physicsSolidAt(const PhysicsField &f, float wx, float wy) {
  for (int i = 0; i < f.count; i++) {
    const PhysicsTile &t = f.tiles[i];
    if (!t.active)
      continue;
    if (wx >= t.pos.x && wx < t.pos.x + 16.f && wy >= t.pos.y &&
        wy < t.pos.y + 16.f)
      return true;
  }
  return false;
}

namespace {

constexpr float ACT_H = 16.f;
constexpr int MAX_ACTORS = 1 + MAX_ENEMIES;

struct SActor {
  Vec2 *pos;
  Vec2 *vel;
  bool *onGround;
  float ox;
  float w;
  Enemy *enemy;
  Player *player;
};

bool tileAtPoint(const PhysicsField &f, int skip, float wx, float wy) {
  for (int i = 0; i < f.count; i++) {
    if (i == skip || !f.tiles[i].active)
      continue;
    const PhysicsTile &o = f.tiles[i];
    if (wx >= o.pos.x && wx < o.pos.x + 16.f && wy >= o.pos.y &&
        wy < o.pos.y + 16.f)
      return true;
  }
  return false;
}

bool spanFree(const Room &room, const PhysicsField &f, int skip, float x0,
              float y0, float x1, float y1) {
  const float e = 0.5f;
  float xs[2] = {x0 + e, x1 - e};
  float ys[2] = {y0 + e, y1 - e};
  for (float x : xs)
    for (float y : ys) {
      if (isSolid(room, x, y))
        return false;
      if (tileAtPoint(f, skip, x, y))
        return false;
    }
  return true;
}

bool overlaps(const SActor &a, const PhysicsTile &b) {
  float ax0 = a.pos->x + a.ox, ax1 = ax0 + a.w;
  float ay0 = a.pos->y, ay1 = ay0 + ACT_H;
  return ax1 > b.pos.x && ax0 < b.pos.x + 16.f && ay1 > b.pos.y &&
         ay0 < b.pos.y + 16.f;
}

bool riding(const SActor &a, const PhysicsTile &b) {
  float ax0 = a.pos->x + a.ox, ax1 = ax0 + a.w;
  float ay1 = a.pos->y + ACT_H;
  return ax1 > b.pos.x + 1.f && ax0 < b.pos.x + 15.f &&
         std::fabs(ay1 - b.pos.y) <= 2.f;
}

bool actorDead(const SActor &a) { return a.enemy && a.enemy->dying; }

void crush(SActor &a) {
  if (!a.enemy)
    return;
  Enemy &e = *a.enemy;
  e.dying = true;
  e.vel = {};
  e.anim = EnemyAnim::DEATH;
  e.animFrame = 0;
  e.animTimer = 0.f;
}

void carryX(SActor &a, float dx, const Room &room, const PhysicsField &f,
            int bi) {
  float nx = a.pos->x + dx;
  if (spanFree(room, f, bi, nx + a.ox, a.pos->y, nx + a.ox + a.w,
               a.pos->y + ACT_H))
    a.pos->x = nx;
}

void carryY(SActor &a, float dy, const Room &room, const PhysicsField &f,
            int bi) {
  float ny = a.pos->y + dy;
  if (spanFree(room, f, bi, a.pos->x + a.ox, ny, a.pos->x + a.ox + a.w,
               ny + ACT_H))
    a.pos->y = ny;
}

bool tryResolve(SActor &a, const PhysicsTile &b, int bi, int axis, int sign,
                const Room &room, const PhysicsField &f) {
  float tx = a.pos->x, ty = a.pos->y;
  if (axis == 0)
    tx = (sign > 0) ? (b.pos.x + 16.f - a.ox) : (b.pos.x - a.ox - a.w);
  else
    ty = (sign > 0) ? (b.pos.y + 16.f) : (b.pos.y - ACT_H);

  if (!spanFree(room, f, bi, tx + a.ox, ty, tx + a.ox + a.w, ty + ACT_H))
    return false;

  a.pos->x = tx;
  a.pos->y = ty;
  if (axis == 0) {
    if (a.vel && a.enemy)
      a.vel->x = 0.f;
  } else {
    if (a.vel)
      a.vel->y = 0.f;
    if (sign < 0) {
      if (a.onGround)
        *a.onGround = true;
      if (a.player) {
        Player &p = *a.player;
        if (!p.isDead)
          p.jumpsLeft = p.hasDoubleJump ? 1 : 0;
        if (p.anim == AnimState::FALL || p.anim == AnimState::JUMP) {
          p.anim = AnimState::IDLE;
          p.animFrame = 0;
          p.animTimer = 0.f;
        }
      }
    }
  }
  return true;
}

bool supportedBelow(const SActor &a, const Room &room, const PhysicsField &f,
                    int skip) {
  float fy = a.pos->y + ACT_H + 0.5f;
  float ax0 = a.pos->x + a.ox + 0.5f;
  float ax1 = a.pos->x + a.ox + a.w - 0.5f;
  return isSolid(room, ax0, fy) || isSolid(room, ax1, fy) ||
         isPlatform(room, ax0, fy) || isPlatform(room, ax1, fy) ||
         tileAtPoint(f, skip, ax0, fy) || tileAtPoint(f, skip, ax1, fy);
}

void resolveActorOutOfTile(SActor &a, PhysicsField &f, int bi,
                           const Room &room) {
  const PhysicsTile &b = f.tiles[bi];
  if (!overlaps(a, b))
    return;

  float ax0 = a.pos->x + a.ox, ax1 = ax0 + a.w;
  float ay0 = a.pos->y, ay1 = ay0 + ACT_H;

  bool grounded = supportedBelow(a, room, f, bi);

  struct Cand {
    float pen;
    int axis;
    int sign;
  };
  Cand c[4] = {
      {ax1 - b.pos.x, 0, -1},
      {(b.pos.x + 16.f) - ax0, 0, +1},
      {ay1 - b.pos.y, 1, -1},
      {(b.pos.y + 16.f) - ay0, 1, +1},
  };
  std::sort(c, c + 4,
            [](const Cand &x, const Cand &y) { return x.pen < y.pen; });

  for (int i = 0; i < 4; i++) {
    if (c[i].axis == 1 && c[i].sign > 0 && grounded)
      continue;
    if (tryResolve(a, b, bi, c[i].axis, c[i].sign, room, f))
      return;
  }

  crush(a);
}

void resolveActorsOutOfTiles(PhysicsField &f, const Room &room, SActor *acts,
                             int na) {
  for (int pass = 0; pass < 2; pass++)
    for (int bi = 0; bi < f.count; bi++) {
      if (!f.tiles[bi].active)
        continue;
      for (int k = 0; k < na; k++) {
        if (actorDead(acts[k]))
          continue;
        resolveActorOutOfTile(acts[k], f, bi, room);
      }
    }
}

void settleTileY(PhysicsField &f, int bi, const Room &room, float dt,
                 SActor *acts, int na) {
  PhysicsTile &b = f.tiles[bi];

  bool wasRiding[MAX_ACTORS];
  for (int k = 0; k < na; k++)
    wasRiding[k] = riding(acts[k], b);

  float prevVelY = b.vel.y;
  b.vel.y = std::min(b.vel.y + PHYS_GRAVITY * dt, PHYS_MAX_FALL);

  float oldY = b.pos.y;
  b.pos.y += b.vel.y * dt;
  b.onGround = false;

  if (b.vel.y >= 0.f) {
    float cy = b.pos.y + 16.f;
    float cx0 = b.pos.x + 1.f, cx1 = b.pos.x + 15.f;
    bool floorHit = isSolid(room, cx0, cy) || isSolid(room, cx1, cy) ||
                    isPlatform(room, cx0, cy) || isPlatform(room, cx1, cy) ||
                    tileAtPoint(f, bi, cx0, cy) || tileAtPoint(f, bi, cx1, cy);
    if (floorHit) {
      b.pos.y = std::floor(cy / 16.f) * 16.f - 16.f;
      b.vel.y = 0.f;
      b.onGround = true;
    }
  } else {
    float cy = b.pos.y;
    if (isSolid(room, b.pos.x + 1.f, cy) || isSolid(room, b.pos.x + 15.f, cy) ||
        tileAtPoint(f, bi, b.pos.x + 1.f, cy) ||
        tileAtPoint(f, bi, b.pos.x + 15.f, cy)) {
      b.pos.y = std::floor(cy / 16.f + 1.f) * 16.f;
      b.vel.y = 0.f;
    }
  }

  float realDy = b.pos.y - oldY;
  if (realDy == 0.f)
    return;

  for (int k = 0; k < na; k++) {
    SActor &a = acts[k];
    if (actorDead(a))
      continue;
    if (overlaps(a, b)) {
      if (realDy > 0.f && prevVelY >= PHYS_CRUSH_VEL && a.enemy)
        crush(a);
    } else if (wasRiding[k]) {
      carryY(a, realDy, room, f, bi);
    }
  }
}

void pushTileX(PhysicsField &f, int bi, float dir, const Room &room,
               SActor *acts, int na, float dt) {
  PhysicsTile &b = f.tiles[bi];

  bool rid[MAX_ACTORS];
  for (int k = 0; k < na; k++)
    rid[k] = riding(acts[k], b);

  float dx = dir * PHYS_PUSH_SPEED * dt;
  b.pos.x += dx;

  float y0 = b.pos.y + 1.f, y1 = b.pos.y + 15.f;
  float corr = 0.f;
  if (dir > 0.f) {
    float R = b.pos.x + 16.f;
    if (isSolid(room, R, y0) || isSolid(room, R, y1))
      corr = std::max(corr, R - std::floor(R / 16.f) * 16.f);
    for (int i = 0; i < f.count; i++) {
      if (i == bi || !f.tiles[i].active)
        continue;
      const PhysicsTile &o = f.tiles[i];
      if (o.pos.y < b.pos.y + 16.f && o.pos.y + 16.f > b.pos.y && o.pos.x < R &&
          o.pos.x + 16.f > b.pos.x)
        corr = std::max(corr, R - o.pos.x);
    }
  } else {
    float Lx = b.pos.x;
    if (isSolid(room, Lx, y0) || isSolid(room, Lx, y1))
      corr = std::min(corr, Lx - (std::floor(Lx / 16.f) + 1.f) * 16.f);
    for (int i = 0; i < f.count; i++) {
      if (i == bi || !f.tiles[i].active)
        continue;
      const PhysicsTile &o = f.tiles[i];
      if (o.pos.y < b.pos.y + 16.f && o.pos.y + 16.f > b.pos.y &&
          o.pos.x < Lx + 16.f && o.pos.x + 16.f > Lx)
        corr = std::min(corr, Lx - (o.pos.x + 16.f));
    }
  }
  if (corr != 0.f)
    b.pos.x -= corr;
  float realDx = dx - corr;

  for (int k = 0; k < na; k++) {
    SActor &a = acts[k];
    if (actorDead(a))
      continue;
    if (rid[k] && !overlaps(a, b))
      carryX(a, realDx, room, f, bi);
  }
}

void tryPlayerPush(PhysicsField &f, const Room &room, SActor *acts, int na,
                   Player &player, float dt) {
  if (player.isDead || std::fabs(player.vel.x) <= 1.f)
    return;

  float dir = (player.vel.x > 0.f) ? 1.f : -1.f;
  float px0 = player.pos.x + 3.f, px1 = px0 + 10.f;
  float py0 = player.pos.y, py1 = py0 + 16.f;
  float pc = px0 + 5.f;

  int head = -1;
  for (int i = 0; i < f.count; i++) {
    PhysicsTile &b = f.tiles[i];
    if (!b.active || !b.pushable)
      continue;
    if (py1 <= b.pos.y + 2.f || py0 >= b.pos.y + 14.f)
      continue;
    if (px1 <= b.pos.x || px0 >= b.pos.x + 16.f)
      continue;
    float bc = b.pos.x + 8.f;
    if (dir > 0.f && bc > pc) {
      if (head < 0 || b.pos.x < f.tiles[head].pos.x)
        head = i;
    } else if (dir < 0.f && bc < pc) {
      if (head < 0 || b.pos.x > f.tiles[head].pos.x)
        head = i;
    }
  }
  if (head >= 0)
    pushTileX(f, head, dir, room, acts, na, dt);
}

} // namespace

void updatePhysicsTiles(PhysicsField &f, const Room &room, Player &player,
                        EnemyField &enemies, float dt) {
  SActor acts[MAX_ACTORS];
  int na = 0;
  if (!player.isDead)
    acts[na++] = {&player.pos,  &player.vel, &player.onGround, PLAYER_BOX_X,
                  PLAYER_BOX_W, nullptr,     &player};
  for (int i = 0; i < enemies.count; i++) {
    Enemy &e = enemies.enemies[i];
    if (e.active && !e.dying)
      acts[na++] = {&e.pos,      &e.vel, &e.onGround, ENEMY_BOX_X,
                    ENEMY_BOX_W, &e,     nullptr};
  }

  int order[MAX_PHYS_TILES];
  int oc = 0;
  for (int i = 0; i < f.count; i++)
    if (f.tiles[i].active)
      order[oc++] = i;
  std::sort(order, order + oc,
            [&](int a, int b) { return f.tiles[a].pos.y > f.tiles[b].pos.y; });
  for (int q = 0; q < oc; q++)
    settleTileY(f, order[q], room, dt, acts, na);

  tryPlayerPush(f, room, acts, na, player, dt);
  resolveActorsOutOfTiles(f, room, acts, na);
}

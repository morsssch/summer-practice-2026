#include "game/actor/enemy.h"
#include "game/world/physics_tile.h"
#include "game/world/tilemap.h"
#include <cmath>

static const float ENEMY_GRAVITY = 500.f;

static bool solidAt(const Room &room, const PhysicsField &st, float x,
                    float y) {
  return isSolid(room, x, y) || physicsSolidAt(st, x, y);
}

static bool groundAt(const Room &room, const PhysicsField &st, float x,
                     float y) {
  return isSolid(room, x, y) || isPlatform(room, x, y) ||
         physicsSolidAt(st, x, y);
}

const EnemyTemplate ENEMY_TEMPLATES[(int)EnemyType::COUNT] = {
    {
        // Гриб
        .maxHp = 2,
        .walkSpeed = 35.f,
        .chaseSpeed = 48.f,
        .jumpVelocity = 0.f,
        .aggroRange = 100.f,
        .attackRange = 20.f,
        .attackCooldown = 1.5f,
        .knockback = 80.f,
        .dropType = -1,
        .spriteAlpha = 255,
        .attackHitboxW = 14.f,
        .attackHitboxH = 10.f,
        .attackBoxR = 12.f,
        .attackBoxL = -14.f,
        .attackHitFrame = 3,
        .attackEffect = AttackEffect::SPORE_CLOUD,
        .hitEndAnim = EnemyAnim::WALK,
        .attackEndAnim = EnemyAnim::WALK,
        .anims = {{8, 0.12f}, {8, 0.12f}, {6, 0.12f}, {3, 0.10f}, {6, 0.12f}},
    },
    {
        // Слизень
        .maxHp = 1,
        .walkSpeed = 0.f,
        .chaseSpeed = 70.f,
        .jumpVelocity = 200.f,
        .aggroRange = 100.f,
        .attackRange = 0.f,
        .attackCooldown = 1.8f,
        .knockback = 80.f,
        .dropType = -1,
        .spriteAlpha = 178,
        .attackHitboxW = 0.f,
        .attackHitboxH = 0.f,
        .attackBoxR = 0.f,
        .attackBoxL = 0.f,
        .attackHitFrame = 99,
        .attackEffect = AttackEffect::NONE,
        .hitEndAnim = EnemyAnim::IDLE,
        .attackEndAnim = EnemyAnim::IDLE,
        .anims = {{5, 0.15f}, {15, 0.07f}, {1, 0.12f}, {3, 0.10f}, {6, 0.12f}},
    },
};

void initEnemies(EnemyField &ef, const Room &room) {
  ef.count = 0;
  for (int row = 0; row < room.h && ef.count < MAX_ENEMIES; row++) {
    for (int col = 0; col < room.w && ef.count < MAX_ENEMIES; col++) {
      uint8_t tile = room.tiles[row][col];
      if (tile < T_ENEMY_LIGHT || tile >= T_ENEMY_LIGHT + (int)EnemyType::COUNT)
        continue;
      Enemy &e = ef.enemies[ef.count++];
      e = Enemy{};
      e.type = tile - T_ENEMY_LIGHT;
      e.active = true;
      e.pos = {(float)(col * TILE_SIZE), (float)(row * TILE_SIZE)};
      e.spawnX = e.pos.x;
      e.anim = EnemyAnim::IDLE;
      const EnemyTemplate &tmpl = ENEMY_TEMPLATES[e.type];
      e.maxHp = tmpl.maxHp;
      e.currentHp = tmpl.maxHp;
      e.aggroRange = tmpl.aggroRange;
      e.attackRange = tmpl.attackRange;
    }
  }
}

static bool solidBelow(const Room &room, const PhysicsField &st, const Enemy &e,
                       float offsetX) {
  float cy = e.pos.y + 16.f;
  return groundAt(room, st, e.pos.x + offsetX, cy);
}

static void tickAnim(Enemy &e, const EnemyTemplate &tmpl, float dt) {
  int idx = (int)e.anim;
  float frameTime = tmpl.anims[idx].frameTime;
  int frames = tmpl.anims[idx].frames;

  e.animTimer += dt;
  if (e.animTimer < frameTime)
    return;
  e.animTimer = 0.f;
  e.animFrame++;
  if (e.animFrame < frames)
    return;
  e.animFrame = 0;
  switch (e.anim) {
  case EnemyAnim::HIT:
    e.anim = tmpl.hitEndAnim;
    if (e.attackCooldown < 0.3f)
      e.attackCooldown = 0.3f;
    break;
  case EnemyAnim::ATTACK:
    e.anim = tmpl.attackEndAnim;
    e.attackHitDone = false;
    break;
  case EnemyAnim::DEATH:
    e.active = false;
    break;
  default:
    break;
  }
}

void updateEnemies(EnemyField &ef, const Room &room, const PhysicsField &st,
                   Vec2 playerPos, float dt) {
  for (int i = 0; i < ef.count; i++) {
    Enemy &e = ef.enemies[i];
    if (!e.active)
      continue;

    if (e.iframeTimer > 0.f)
      e.iframeTimer -= dt;
    if (e.attackCooldown > 0.f)
      e.attackCooldown -= dt;

    const EnemyTemplate &tmpl = ENEMY_TEMPLATES[e.type];

    if (e.dying) {
      tickAnim(e, tmpl, dt);
      continue;
    }

    e.vel.y += ENEMY_GRAVITY * dt;

    float dx = playerPos.x + 8.f - (e.pos.x + 8.f);
    float dy = playerPos.y + 8.f - (e.pos.y + 8.f);
    float dist = std::abs(dx);
    float distY = std::abs(dy);
    bool inFront = e.facingRight ? (dx > 0.f) : (dx < 0.f);
    if (dist < e.aggroRange && distY < 32.f && inFront)
      e.aggro = true;
    if (dist > e.aggroRange * 1.5f || distY > 48.f)
      e.aggro = false;

    if ((EnemyType)e.type == EnemyType::SLIME) {
      if (e.anim == EnemyAnim::HIT) {
        tickAnim(e, tmpl, dt);
      } else if (e.anim == EnemyAnim::WALK) {
        tickAnim(e, tmpl, dt);
      } else if (e.aggro && e.onGround && e.attackCooldown <= 0.f) {
        float dir = (dx > 0.f) ? 1.f : -1.f;
        e.vel.x = tmpl.chaseSpeed * dir;
        e.vel.y = -tmpl.jumpVelocity;
        e.facingRight = (dir > 0.f);
        e.anim = EnemyAnim::WALK;
        e.animFrame = 0;
        e.animTimer = 0.f;
      } else if (!e.aggro && e.onGround && e.attackCooldown <= 0.f) {
        bool safe = solidBelow(room, st, e, e.patrolDir > 0.f ? 32.f : -16.f);
        if (!safe)
          e.patrolDir = -e.patrolDir;
        e.vel.x = tmpl.chaseSpeed * 0.35f * e.patrolDir;
        e.vel.y = -tmpl.jumpVelocity;
        e.facingRight = (e.patrolDir > 0.f);
        e.anim = EnemyAnim::WALK;
        e.animFrame = 0;
        e.animTimer = 0.f;
      } else {
        e.facingRight = (dx > 0.f);
        e.vel.x = 0.f;
        tickAnim(e, tmpl, dt);
      }
    } else {
      if (e.anim == EnemyAnim::ATTACK) {
        e.vel.x = 0.f;
        tickAnim(e, tmpl, dt);
      } else if (e.anim == EnemyAnim::HIT) {
        tickAnim(e, tmpl, dt);
      } else if (e.aggro && dist <= e.attackRange && e.attackCooldown <= 0.f) {
        e.anim = EnemyAnim::ATTACK;
        e.animFrame = 0;
        e.animTimer = 0.f;
        e.attackCooldown = tmpl.attackCooldown;
        e.attackHitDone = false;
        e.attackFxSpawned = false;
        e.vel.x = 0.f;
        e.facingRight = (dx > 0.f);
      } else if (e.aggro) {
        float dir = (dx > 0.f) ? 1.f : -1.f;
        float frontX = e.pos.x + (dir > 0 ? 15.f : 0.f);
        bool wallHit = solidAt(room, st, frontX, e.pos.y + 6.f);
        bool noGround =
            e.onGround && !solidBelow(room, st, e, dir > 0 ? 16.f : -2.f);
        e.vel.x = (!wallHit && !noGround) ? tmpl.chaseSpeed * dir : 0.f;
        e.facingRight = (dir > 0.f);
        tickAnim(e, tmpl, dt);
      } else {
        float frontX = e.pos.x + (e.patrolDir > 0 ? 15.f : 0.f);
        bool wallHit = solidAt(room, st, frontX, e.pos.y + 6.f);
        bool noGround = e.onGround &&
                        !solidBelow(room, st, e, e.patrolDir > 0 ? 16.f : -2.f);
        bool outOfRange =
            (e.patrolDir > 0 && e.pos.x > e.spawnX + e.patrolRadius) ||
            (e.patrolDir < 0 && e.pos.x < e.spawnX - e.patrolRadius);
        if (wallHit || noGround || outOfRange)
          e.patrolDir = -e.patrolDir;
        e.vel.x = tmpl.walkSpeed * e.patrolDir;
        e.facingRight = (e.patrolDir > 0.f);
        tickAnim(e, tmpl, dt);
      }
    }

    e.pos.x += e.vel.x * dt;
    if (e.vel.x < 0 && isSolid(room, e.pos.x, e.pos.y + 4.f)) {
      e.pos.x = (float)((int)(e.pos.x / TILE_SIZE) + 1) * TILE_SIZE;
      e.vel.x = 0.f;
      e.patrolDir = 1.f;
      e.facingRight = true;
    }
    if (e.vel.x > 0 && isSolid(room, e.pos.x + 14.f, e.pos.y + 4.f)) {
      e.pos.x = (float)((int)((e.pos.x + 14.f) / TILE_SIZE)) * TILE_SIZE - 14.f;
      e.vel.x = 0.f;
      e.patrolDir = -1.f;
      e.facingRight = false;
    }

    e.pos.y += e.vel.y * dt;
    if (e.vel.y < 0 && solidAt(room, st, e.pos.x + 4.f, e.pos.y)) {
      e.pos.y = (float)((int)(e.pos.y / TILE_SIZE) + 1) * TILE_SIZE;
      e.vel.y = 0.f;
    }
    bool landSolid = solidAt(room, st, e.pos.x + 4.f, e.pos.y + 16.f) ||
                     solidAt(room, st, e.pos.x + 10.f, e.pos.y + 16.f);
    bool landPlat = isPlatform(room, e.pos.x + 4.f, e.pos.y + 16.f) ||
                    isPlatform(room, e.pos.x + 10.f, e.pos.y + 16.f);
    if (e.vel.y > 0 && (landSolid || landPlat)) {
      e.pos.y = (float)((int)((e.pos.y + 16.f) / TILE_SIZE) * TILE_SIZE) - 16.f;
      e.vel.y = 0.f;
      e.onGround = true;
    } else {
      e.onGround = false;
    }

    if ((EnemyType)e.type == EnemyType::SLIME && e.onGround &&
        e.anim == EnemyAnim::WALK) {
      e.anim = EnemyAnim::IDLE;
      e.animFrame = 0;
      e.animTimer = 0.f;
      e.vel.x = 0.f;
      e.attackCooldown = tmpl.attackCooldown;
    }
  }
}

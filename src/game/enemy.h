#pragma once
#include <cstdint>
#include "core/types.h"
#include "game/room.h"

enum class EnemyType  { MUSHROOM = 0, SLIME = 1, COUNT };
enum class EnemyAnim  { IDLE, WALK, ATTACK, HIT, DEATH };
enum class AttackEffect { NONE, SPORE_CLOUD };

struct EnemyTemplate {
    int          maxHp;
    float        walkSpeed;
    float        chaseSpeed;
    float        jumpVelocity;
    float        aggroRange;
    float        attackRange;
    float        attackCooldown;
    float        knockback;
    int          dropType;
    uint8_t      spriteAlpha;
    float        attackHitboxW;
    float        attackHitboxH;
    float        attackBoxR;
    float        attackBoxL;
    int          attackHitFrame;
    AttackEffect attackEffect;
    EnemyAnim    hitEndAnim;
    EnemyAnim    attackEndAnim;
    struct AnimInfo { int frames; float frameTime; } anims[5];
};

extern const EnemyTemplate ENEMY_TEMPLATES[(int)EnemyType::COUNT];

struct Enemy {
    Vec2      pos          = {};
    Vec2      vel          = {};
    float     spawnX       = 0.f;
    float     patrolRadius = 64.f;
    int       maxHp        = 2;
    int       currentHp    = 2;
    float     iframeTimer  = 0.f;
    float     patrolDir    = 1.f;
    float     aggroRange   = 100.f;
    float     attackRange  = 20.f;
    float     attackCooldown = 0.f;
    int       type           = 0;
    bool      active         = false;
    bool      onGround       = false;
    bool      dying          = false;
    bool      aggro          = false;
    bool      attackHitDone  = false;
    bool      facingRight    = true;
    bool      attackFxSpawned = false;
    bool      dropSpawned     = false;
    EnemyAnim anim           = EnemyAnim::IDLE;
    int       animFrame      = 0;
    float     animTimer      = 0.f;
};

constexpr int MAX_ENEMIES = 16;

struct EnemyField {
    Enemy enemies[MAX_ENEMIES];
    int   count = 0;
};

void initEnemies(EnemyField& ef, const Room& room);
void updateEnemies(EnemyField& ef, const Room& room, Vec2 playerPos, float dt);

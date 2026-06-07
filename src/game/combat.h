#pragma once
#include "game/player.h"
#include "game/enemy.h"

struct HitBox { float x, y, w, h; };

inline bool overlaps(HitBox a, HitBox b) {
    return a.x < b.x + b.w && a.x + a.w > b.x
        && a.y < b.y + b.h && a.y + a.h > b.y;
}

struct PlayerAttackProps { float hitboxW; float knockback; };

HitBox getAttackHitBox(const Player& p, const PlayerAttackProps& props);
void   resolveAttack(Player& p, EnemyField& ef, const PlayerAttackProps& props);
void   resolveEnemyContact(EnemyField& ef, Player& p);
void   resolveEnemyAttack(EnemyField& ef, Player& p);

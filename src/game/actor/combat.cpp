#include "game/actor/combat.h"

namespace {
constexpr float ATTACK_BOX_X = 13.f, ATTACK_BOX_Y = 2.f, ATTACK_BOX_H = 12.f;
constexpr float ENEMY_HURT_W = 14.f, ENEMY_HURT_H = 14.f;
constexpr float PLAYER_HURT_X = 2.f, PLAYER_HURT_Y = 1.f, PLAYER_HURT_W = 12.f,
                PLAYER_HURT_H = 14.f;
constexpr float ENEMY_CONTACT_X = 1.f, ENEMY_CONTACT_Y = 1.f,
                ENEMY_CONTACT_W = 12.f, ENEMY_CONTACT_H = 12.f;

HitBox playerHurtBox(const Player &p) {
  return {p.pos.x + PLAYER_HURT_X, p.pos.y + PLAYER_HURT_Y, PLAYER_HURT_W,
          PLAYER_HURT_H};
}
}

HitBox getAttackHitBox(const Player &p, const PlayerAttackProps &props) {
  float x = p.attackFxFacing ? p.pos.x + ATTACK_BOX_X : p.pos.x - props.hitboxW;
  return {x, p.pos.y + ATTACK_BOX_Y, props.hitboxW, ATTACK_BOX_H};
}

void resolveAttack(Player &p, EnemyField &ef, const PlayerAttackProps &props) {
  if (p.anim != AnimState::ATTACK || p.attackHitDone)
    return;
  HitBox atk = getAttackHitBox(p, props);
  bool anyHit = false;
  for (int i = 0; i < ef.count; i++) {
    Enemy &e = ef.enemies[i];
    if (!e.active || e.dying || e.iframeTimer > 0.f)
      continue;
    HitBox ehb = {e.pos.x, e.pos.y, ENEMY_HURT_W, ENEMY_HURT_H};
    if (!overlaps(atk, ehb))
      continue;
    e.currentHp--;
    e.iframeTimer = 0.5f;
    e.vel.x = p.attackFxFacing ? props.knockback : -props.knockback;
    e.vel.y = -100.f;
    if (e.currentHp <= 0) {
      e.dying = true;
      e.anim = EnemyAnim::DEATH;
      e.animFrame = 0;
      e.animTimer = 0.f;
    } else {
      e.anim = EnemyAnim::HIT;
      e.animFrame = 0;
      e.animTimer = 0.f;
    }
    anyHit = true;
  }
  if (anyHit)
    p.attackHitDone = true;
}

void resolveEnemyAttack(EnemyField &ef, Player &p) {
  if (p.isDead || p.pendingDeath || p.rollTimer > 0.f || p.rollGraceTimer > 0.f)
    return;
  for (int i = 0; i < ef.count; i++) {
    Enemy &e = ef.enemies[i];
    if (!e.active || e.dying || e.anim != EnemyAnim::ATTACK || e.attackHitDone)
      continue;
    const EnemyTemplate &tmpl = ENEMY_TEMPLATES[e.type];
    if (e.animFrame < tmpl.attackHitFrame)
      continue;
    float ax =
        e.facingRight ? e.pos.x + tmpl.attackBoxR : e.pos.x + tmpl.attackBoxL;
    HitBox atk = {ax, e.pos.y + 2.f, tmpl.attackHitboxW, tmpl.attackHitboxH};
    HitBox phb = playerHurtBox(p);
    if (!overlaps(atk, phb))
      continue;
    float kb = e.facingRight ? tmpl.knockback * 1.5f : -tmpl.knockback * 1.5f;
    damagePlayer(p, 1, kb);
    e.attackHitDone = true;
  }
}

void resolveEnemyContact(EnemyField &ef, Player &p) {
  if (p.isDead || p.pendingDeath || p.rollTimer > 0.f || p.rollGraceTimer > 0.f)
    return;
  HitBox phb = playerHurtBox(p);
  for (int i = 0; i < ef.count; i++) {
    const Enemy &e = ef.enemies[i];
    if (!e.active || e.dying)
      continue;
    HitBox ehb = {e.pos.x + ENEMY_CONTACT_X, e.pos.y + ENEMY_CONTACT_Y,
                  ENEMY_CONTACT_W, ENEMY_CONTACT_H};
    if (!overlaps(phb, ehb))
      continue;
    float kb = (p.pos.x < e.pos.x) ? -150.f : 150.f;
    damagePlayer(p, 1, kb);
    break;
  }
}

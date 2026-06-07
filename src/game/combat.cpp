#include "combat.h"

HitBox getAttackHitBox(const Player& p, const PlayerAttackProps& props) {
    float x = p.attackFxFacing ? p.pos.x + 13.f : p.pos.x - props.hitboxW;
    return { x, p.pos.y + 2.f, props.hitboxW, 12.f };
}

void resolveAttack(Player& p, EnemyField& ef, const PlayerAttackProps& props) {
    if (p.anim != AnimState::ATTACK || p.attackHitDone) return;
    HitBox atk = getAttackHitBox(p, props);
    bool anyHit = false;
    for (int i = 0; i < ef.count; i++) {
        Enemy& e = ef.enemies[i];
        if (!e.active || e.dying || e.iframeTimer > 0.f) continue;
        HitBox ehb = { e.pos.x, e.pos.y, 14.f, 14.f };
        if (!overlaps(atk, ehb)) continue;
        e.currentHp--;
        e.iframeTimer = 0.5f;
        e.vel.x = p.attackFxFacing ? props.knockback : -props.knockback;
        e.vel.y = -100.f;
        if (e.currentHp <= 0) {
            e.dying     = true;
            e.anim      = EnemyAnim::DEATH;
            e.animFrame = 0;
            e.animTimer = 0.f;
        } else {
            e.anim      = EnemyAnim::HIT;
            e.animFrame = 0;
            e.animTimer = 0.f;
        }
        anyHit = true;
    }
    if (anyHit) p.attackHitDone = true;
}

void resolveEnemyAttack(EnemyField& ef, Player& p) {
    if (p.isDead || p.pendingDeath || p.rollTimer > 0.f || p.rollGraceTimer > 0.f) return;
    for (int i = 0; i < ef.count; i++) {
        Enemy& e = ef.enemies[i];
        if (!e.active || e.dying || e.anim != EnemyAnim::ATTACK || e.attackHitDone) continue;
        const EnemyTemplate& tmpl = ENEMY_TEMPLATES[e.type];
        if (e.animFrame < tmpl.attackHitFrame) continue;
        float ax  = e.facingRight ? e.pos.x + tmpl.attackBoxR : e.pos.x + tmpl.attackBoxL;
        HitBox atk = { ax, e.pos.y + 2.f, tmpl.attackHitboxW, tmpl.attackHitboxH };
        HitBox phb = { p.pos.x + 2.f, p.pos.y + 1.f, 12.f, 14.f };
        if (!overlaps(atk, phb)) continue;
        float kb = e.facingRight ? tmpl.knockback * 1.5f : -tmpl.knockback * 1.5f;
        damagePlayer(p, 1, kb);
        e.attackHitDone = true;
    }
}

void resolveEnemyContact(EnemyField& ef, Player& p) {
    if (p.isDead || p.pendingDeath || p.rollTimer > 0.f || p.rollGraceTimer > 0.f) return;
    HitBox phb = { p.pos.x + 2.f, p.pos.y + 1.f, 12.f, 14.f };
    for (int i = 0; i < ef.count; i++) {
        const Enemy& e = ef.enemies[i];
        if (!e.active || e.dying) continue;
        HitBox ehb = { e.pos.x + 1.f, e.pos.y + 1.f, 12.f, 12.f };
        if (!overlaps(phb, ehb)) continue;
        float kb = (p.pos.x < e.pos.x) ? -150.f : 150.f;
        damagePlayer(p, 1, kb);
        break;
    }
}

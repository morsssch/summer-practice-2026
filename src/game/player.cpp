#include "player.h"
#include "tilemap.h"
#include <algorithm>

const float ACCEL    = 800.f;
const float FRIC     = 600.f;
const float MAX_VEL  = 120.f;
const float GRAVITY  = 500.f;
const float JUMP_VEL = 300.f;
const float ROLL_DUR = 0.35f;
const float ROLL_CD  = 0.7f;
const float ATTACK_DUR = 4 * 0.1f;
const float ATTACK_CD  = 0.7f;

static void updatePhysics(Player& p, const Input& input, float dt,
                           const Room& room, bool wasOnGround)
{
    if (p.isDead) {
        p.deathFrameTimer += dt;
        if (p.deathFrameTimer >= 0.12f) {
            p.deathFrameTimer = 0.f;
            if (p.deathFrame < 7) p.deathFrame++;
        }
        p.deathTimer -= dt;
        return;
    }
    if (p.iframeTimer       > 0.f) p.iframeTimer       -= dt;
    if (p.rollGraceTimer    > 0.f) p.rollGraceTimer    -= dt;
    if (p.facingCoyoteTimer > 0.f) p.facingCoyoteTimer -= dt;

    if (p.pendingDeath && p.iframeTimer <= 0.f) {
        p.pendingDeath    = false;
        p.isDead          = true;
        p.deathTimer      = 8 * 0.12f;
        p.deathFrame      = 0;
        p.deathFrameTimer = 0.f;
        p.vel             = {0.f, 0.f};
        return;
    }
    if (input.jumpPressed)       p.jumpBuffer      = 0.12f;
    if (p.jumpBuffer   > 0.f)   p.jumpBuffer      -= dt;
    if (p.coyoteTimer  > 0.f)   p.coyoteTimer     -= dt;
    if (p.rollCooldown > 0.f)   p.rollCooldown    -= dt;
    if (p.attackTimer > 0.f)    p.attackTimer     -= dt;
    if (p.attackCooldown > 0.f) p.attackCooldown  -= dt;

    bool canJump    = p.onGround || p.coyoteTimer > 0.f;
    bool canAirJump = !canJump && p.jumpsLeft > 0;
    bool rolling    = p.rollTimer > 0.f;

    if (rolling) {
        p.rollTimer -= dt;
        if (p.rollTimer <= 0.f) p.rollGraceTimer = 0.12f;
    } else {
        bool prevFacing = p.facingRight;
        if (input.right) p.facingRight = true;
        if (input.left)  p.facingRight = false;
        if (p.facingRight != prevFacing) {
            p.lastFacingRight   = prevFacing;
            p.facingCoyoteTimer = 0.15f;
        }

        if (input.attackPressed && p.attackTimer <= 0.f && p.attackCooldown <= 0.f) {
            p.attackTimer    = ATTACK_DUR;
            p.attackCooldown = ATTACK_CD;
            bool atkDir      = (p.facingCoyoteTimer > 0.f) ? p.lastFacingRight : p.facingRight;
            p.attackFxFacing = atkDir;
            p.attackFxPos    = { atkDir ? p.pos.x + 16.f : p.pos.x - 16.f, p.pos.y };
        }

        p.vel.x -= ACCEL * (input.left)  * dt;
        p.vel.x += ACCEL * (input.right) * dt;

        if (!input.left && !input.right) {
            if (p.vel.x > 0) p.vel.x = std::max(0.f, p.vel.x - FRIC * dt);
            if (p.vel.x < 0) p.vel.x = std::min(0.f, p.vel.x + FRIC * dt);
        }

        p.vel.x = std::max(-MAX_VEL, std::min(MAX_VEL, p.vel.x));

        if (p.attackTimer <= 0.f) p.attackHitDone = false;

        if (p.jumpBuffer > 0.f && (canJump || canAirJump)) {
            p.vel.y       = -JUMP_VEL;
            if (canAirJump) p.jumpsLeft--;
            p.coyoteTimer = 0.f;
            p.jumpBuffer  = 0.f;
            p.jumpFxPos   = { p.pos.x, p.pos.y + 6.f };
            p.jumpFxTimer = 0.f;
        }

        if (input.rollPressed && p.onGround && p.rollCooldown <= 0.f) {
            p.rollTimer    = ROLL_DUR;
            p.rollCooldown = ROLL_CD;
            p.attackTimer  = 0.f;
        }
    }

    p.pos.x += p.vel.x * dt;

    if (p.vel.x < 0 && (isSolid(room, p.pos.x+3, p.pos.y+2) || isSolid(room, p.pos.x+3, p.pos.y+12))) {
        p.pos.x = (float)((int)((p.pos.x+3) / 16) + 1) * 16 - 3;
        p.vel.x = 0.f;
    }

    if (p.vel.x > 0 && (isSolid(room, p.pos.x+13, p.pos.y+2) || isSolid(room, p.pos.x+13, p.pos.y+12))) {
        p.pos.x = (float)((int)((p.pos.x+13) / 16)) * 16 - 13;
        p.vel.x = 0.f;
    }

    p.vel.y += GRAVITY * dt;

    float prevBottom = p.pos.y + 16;
    p.pos.y += p.vel.y * dt;

    if (p.vel.y < 0 && (isSolid(room, p.pos.x+4, p.pos.y) || isSolid(room, p.pos.x+12, p.pos.y))) {
        p.pos.y = (float)((int)(p.pos.y / 16) + 1) * 16;
        p.vel.y = 0.f;
    }
    if (p.vel.y > 0 && (isSolid(room, p.pos.x+4, p.pos.y+16) || isSolid(room, p.pos.x+12, p.pos.y+16))) {
        p.pos.y    = (float)((int)((p.pos.y+16) / 16) * 16) - 16;
        p.vel.y    = 0.f;
        p.onGround = true;
    } else if (p.vel.y > 0
        && (isPlatform(room, p.pos.x+4, p.pos.y+16) || isPlatform(room, p.pos.x+12, p.pos.y+16))
        && prevBottom <= (float)((int)((p.pos.y+16) / 16) * 16))
    {
        p.pos.y    = (float)((int)((p.pos.y+16) / 16) * 16) - 16;
        p.vel.y    = 0.f;
        p.onGround = true;
    } else {
        p.onGround = false;
    }

    if (wasOnGround && !p.onGround && p.vel.y >= 0.f)
        p.coyoteTimer = 0.1f;
}

static void updateAnim(Player& p, const Input& input, float dt)
{
    if (p.landTimer > 0.f) p.landTimer -= dt;

    AnimState next;
    int       frames;
    float     frameTime;

    if (p.rollTimer > 0.f) {
        next = AnimState::ROLL;   frames = 3; frameTime = ROLL_DUR / 3.f;
    } else if (p.attackTimer > 0.f) {
        next = AnimState::ATTACK; frames = 4; frameTime = 0.1f;
    } else if (p.landTimer > 0.f) {
        next = AnimState::LAND; frames = 2; frameTime = 0.1f;
    } else if (!p.onGround && p.vel.y < 0.f) {
        next = AnimState::JUMP; frames = 3; frameTime = 0.08f;
    } else if (!p.onGround && p.vel.y >= 0.f) {
        next = AnimState::FALL; frames = 3; frameTime = 0.08f;
    } else if (input.left || input.right) {
        next = AnimState::RUN;  frames = 6; frameTime = 0.06f;
    } else {
        next = AnimState::IDLE; frames = 4; frameTime = 0.15f;
    }

    if (p.anim != next) {
        p.anim      = next;
        p.animFrame = 0;
        p.animTimer = 0.f;
    }

    p.animTimer += dt;
    if (p.animTimer >= frameTime) {
        p.animTimer = 0.f;
        p.animFrame = (p.animFrame + 1) % frames;
    }
}

void updatePlayer(Player& p, const Input& input, const Room& room, float dt)
{
    bool wasOnGround = p.onGround;
    updatePhysics(p, input, dt, room, wasOnGround);
    if (!wasOnGround && p.onGround) {
        p.landTimer  = 0.2f;
        p.jumpsLeft  = p.hasDoubleJump ? 1 : 0;
    }
    if (p.jumpFxTimer >= 0.f) {
        p.jumpFxTimer += dt;
        if (p.jumpFxTimer >= 4 * 0.07f) p.jumpFxTimer = -1.f;
    }
    if (p.hitAnimTimer >= 0.f) {
        p.hitAnimTimer += dt;
        if (p.hitAnimTimer >= 0.4f) p.hitAnimTimer = -1.f;
    }
    if (p.lostHeartTimer >= 0.f) {
        p.lostHeartFrameTimer += dt;
        if (p.lostHeartFrameTimer >= 0.08f) {
            p.lostHeartFrameTimer = 0.f;
            p.lostHeartFrame++;
            if (p.lostHeartFrame >= 5) p.lostHeartTimer = -1.f;
        }
    }
    updateAnim(p, input, dt);
}

void damagePlayer(Player& p, int amount, float knockbackX) {
    if (p.iframeTimer > 0.f || p.isDead || p.pendingDeath) return;
    p.currentHp = std::max(0, p.currentHp - amount);
    p.vel.x          = knockbackX;
    p.vel.y          = -180.f;
    p.lostHeartSlot  = p.currentHp;
    p.lostHeartTimer = 0.f;
    p.lostHeartFrame = 0;
    p.lostHeartFrameTimer = 0.f;
    p.hitAnimTimer   = 0.f;
    if (p.currentHp <= 0) {
        p.pendingDeath = true;
        p.iframeTimer  = 0.6f;
        return;
    }
    p.iframeTimer = 1.2f;
}

#include "player.h"
#include "tilemap.h"
#include <algorithm>

const float ACCEL    = 800.f;
const float FRIC     = 600.f;
const float MAX_VEL  = 120.f;
const float GRAVITY  = 500.f;
const float JUMP_VEL = 280.f;
const float ROLL_DUR = 0.35f;
const float ROLL_CD  = 0.7f;
const float ATTACK_DUR = 4 * 0.1f;
const float ATTACK_CD  = 0.7f;

static void updatePhysics(Player& p, const Input& input, float dt,
                           const uint8_t tiles[17][30], bool wasOnGround)
{
    // Таймеры
    if (input.jumpPressed)      p.jumpBuffer     = 0.12f;
    if (p.jumpBuffer   > 0.f)   p.jumpBuffer     -= dt;
    if (p.coyoteTimer  > 0.f)   p.coyoteTimer    -= dt;
    if (p.rollCooldown > 0.f)   p.rollCooldown   -= dt;
    if (p.attackTimer > 0.f)    p.attackTimer    -= dt;
    if (p.attackCooldown > 0.f) p.attackCooldown -= dt;

    bool canJump = p.onGround || p.coyoteTimer > 0.f;
    bool rolling = p.rollTimer > 0.f;

    if (rolling) {
        p.rollTimer -= dt;
    } else {
        // Атака
        if (input.attackPressed && p.attackTimer <= 0.f && p.attackCooldown <= 0.f) {
            p.attackTimer = ATTACK_DUR;
            p.attackCooldown = ATTACK_CD;
        }

        // Горизонтальное движение
        p.vel.x -= ACCEL * (input.left)  * dt;
        p.vel.x += ACCEL * (input.right) * dt;

        if (input.right) p.facingRight = true;
        if (input.left)  p.facingRight = false;

        if (!input.left && !input.right) {
            if (p.vel.x > 0) p.vel.x = std::max(0.f, p.vel.x - FRIC * dt);
            if (p.vel.x < 0) p.vel.x = std::min(0.f, p.vel.x + FRIC * dt);
        }

        p.vel.x = std::max(-MAX_VEL, std::min(MAX_VEL, p.vel.x));

        // Прыжок
        if (p.jumpBuffer > 0.f && canJump) {
            p.vel.y       = -JUMP_VEL;
            p.coyoteTimer = 0.f;
            p.jumpBuffer  = 0.f;
        }

        // Перекат
        if (input.roll && p.onGround && p.rollCooldown <= 0.f) {
            p.rollTimer    = ROLL_DUR;
            p.rollCooldown = ROLL_CD;
            p.attackTimer  = 0.f;
        }
    }

    // Ускорение
    p.pos.x += p.vel.x * dt;

    // Коллизия по горизонтали
    if (p.vel.x < 0 && (isSolid(tiles, p.pos.x+3, p.pos.y+2) || isSolid(tiles, p.pos.x+3, p.pos.y+12))) {
        p.pos.x = (float)((int)((p.pos.x+3) / 16) + 1) * 16 - 3;
        p.vel.x = 0.f;
    }

    if (p.vel.x > 0 && (isSolid(tiles, p.pos.x+13, p.pos.y+2) || isSolid(tiles, p.pos.x+13, p.pos.y+12))) {
        p.pos.x = (float)((int)((p.pos.x+13) / 16)) * 16 - 13;
        p.vel.x = 0.f;
    }

    p.vel.y += GRAVITY * dt;

    float prevBottom = p.pos.y + 14;
    p.pos.y += p.vel.y * dt;

    // Коллизия по вертикали
    if (p.vel.y < 0 && (isSolid(tiles, p.pos.x+4, p.pos.y) || isSolid(tiles, p.pos.x+12, p.pos.y))) {
        p.pos.y = (float)((int)(p.pos.y / 16) + 1) * 16;
        p.vel.y = 0.f;
    }
    if (p.vel.y > 0 && (isSolid(tiles, p.pos.x+4, p.pos.y+14) || isSolid(tiles, p.pos.x+12, p.pos.y+14))) {
        p.pos.y    = (float)((int)((p.pos.y+14) / 16) * 16) - 14;
        p.vel.y    = 0.f;
        p.onGround = true;
    } else if (p.vel.y > 0
        && (isPlatform(tiles, p.pos.x+4, p.pos.y+14) || isPlatform(tiles, p.pos.x+12, p.pos.y+14))
        && prevBottom <= (float)((int)((p.pos.y+14) / 16) * 16))
    {
        p.pos.y    = (float)((int)((p.pos.y+14) / 16) * 16) - 14;
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
        next = AnimState::LAND; frames = 3; frameTime = 0.07f;
    } else if (!p.onGround && p.vel.y < 0.f) {
        next = AnimState::JUMP; frames = 3; frameTime = 0.08f;
    } else if (!p.onGround && p.vel.y >= 0.f) {
        next = AnimState::FALL; frames = 3; frameTime = 0.08f;
    } else if (input.left || input.right) {
        next = AnimState::RUN;  frames = 6; frameTime = 0.08f;
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

void updatePlayer(Player& p, const Input& input, const uint8_t tiles[17][30], float dt)
{
    bool wasOnGround = p.onGround;
    updatePhysics(p, input, dt, tiles, wasOnGround);
    if (!wasOnGround && p.onGround) p.landTimer = 0.2f;
    updateAnim(p, input, dt);
}

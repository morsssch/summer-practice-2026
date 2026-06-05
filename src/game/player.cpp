#include "player.h"
#include "render/renderer.h"
#include "render/sprites.h"
#include <algorithm>

const float ACCEL    = 800.f;
const float FRIC     = 600.f;
const float MAX_VEL  = 120.f;
const float GRAVITY  = 500.f;
const float JUMP_VEL = 280.f;
const float ROLL_DUR = 0.35f;
const float ROLL_CD  = 0.7f;

static void updatePhysics(Player& p, const Input& input, float dt)
{
    bool rolling = p.rollTimer > 0.f;

    if (!rolling) {
        p.vel.x -= ACCEL * (input.left)  * dt;
        p.vel.x += ACCEL * (input.right) * dt;

        if (input.right) p.facingRight = true;
        if (input.left)  p.facingRight = false;

        if (!input.left && !input.right) {
            if (p.vel.x > 0) p.vel.x = std::max(0.f, p.vel.x - FRIC * dt);
            if (p.vel.x < 0) p.vel.x = std::min(0.f, p.vel.x + FRIC * dt);
        }

        p.vel.x = std::max(-MAX_VEL, std::min(MAX_VEL, p.vel.x));

        if (input.jumpPressed && p.onGround)
            p.vel.y = -JUMP_VEL;

        if (input.roll && p.onGround && p.rollCooldown <= 0.f) {
            p.rollTimer    = ROLL_DUR;
            p.rollCooldown = ROLL_CD;
        }
    } else {
        p.rollTimer -= dt;
    }

    p.pos.x += p.vel.x * dt;
    p.vel.y += GRAVITY * dt;
    p.pos.y += p.vel.y * dt;

    // TODO: заменить на коллизию с тайлмапой
    if (p.pos.y > 304.f) {
        p.pos.y    = 304.f;
        p.vel.y    = 0.f;
        p.onGround = true;
    } else {
        p.onGround = false;
    }
}


static void updateAnim(Player& p, const Input& input, float dt)
{
    if (p.landTimer    > 0.f) p.landTimer    -= dt;
    if (p.rollCooldown > 0.f) p.rollCooldown -= dt;

    AnimState next;
    int       frames;
    float     frameTime;

    if (p.rollTimer > 0.f) {
        next = AnimState::ROLL; frames = 3; frameTime = ROLL_DUR / 3.f;
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

void updatePlayer(Player& p, const Input& input, float dt)
{
    bool wasOnGround = p.onGround;
    updatePhysics(p, input, dt);
    if (!wasOnGround && p.onGround) p.landTimer = 0.2f;
    updateAnim(p, input, dt);
}

void drawPlayer(Renderer& r, const Player& p)
{
    const SpriteFrame* anim;
    switch (p.anim) {
        case AnimState::RUN:  anim = HERO_RUN;  break;
        case AnimState::JUMP: anim = HERO_JUMP; break;
        case AnimState::FALL: anim = HERO_FALL; break;
        case AnimState::LAND: anim = HERO_LAND; break;
        case AnimState::ROLL: anim = HERO_ROLL; break;
        default:              anim = HERO_IDLE; break;
    }
    drawSprite(r, anim[p.animFrame], p.pos.x, p.pos.y, !p.facingRight);
}

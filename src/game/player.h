#pragma once
#include "core/types.h"
#include "core/input.h"
#include "game/room.h"

enum class AnimState { IDLE, RUN, JUMP, FALL, LAND, ROLL, ATTACK };

struct Player {
    Vec2 pos;
    Vec2 vel;
    bool      onGround     = false;
    bool      facingRight  = true;
    int       animFrame    = 0;
    float     animTimer    = 0.f;
    float     landTimer    = 0.f;
    float     rollTimer    = 0.f;
    float     rollCooldown = 0.f;
    float     coyoteTimer  = 0.f;
    float     jumpBuffer   = 0.f;
    float     attackTimer    = 0.f;
    float     attackCooldown = 0.f;
    Vec2      jumpFxPos;
    float     jumpFxTimer  = -1.f;
    AnimState anim         = AnimState::IDLE;
};

void updatePlayer(Player& p, const Input& input, const Room& room, float dt);

#pragma once
#include "core/types.h"
#include "core/input.h"

struct Renderer;

enum class AnimState { IDLE, RUN, JUMP, FALL, LAND, ROLL };

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
    AnimState anim         = AnimState::IDLE;
};

void updatePlayer(Player& p, const Input& input, float dt);
void drawPlayer(Renderer& r, const Player& p);

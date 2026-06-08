#pragma once
#include "core/input.h"
#include "core/types.h"
#include "game/world/room.h"

enum class AnimState { IDLE, RUN, JUMP, FALL, LAND, ROLL, ATTACK };

constexpr float PLAYER_BOX_X = 3.f;
constexpr float PLAYER_BOX_W = 10.f; 
constexpr float PLAYER_BOX_R = 13.f;
constexpr float PLAYER_BOX_H = 16.f;

struct Player {
  Vec2 pos;
  Vec2 vel;
  bool onGround = false;
  bool facingRight = true;
  int animFrame = 0;
  float animTimer = 0.f;
  float landTimer = 0.f;
  float rollTimer = 0.f;
  float rollCooldown = 0.f;
  float coyoteTimer = 0.f;
  float jumpBuffer = 0.f;
  float attackTimer = 0.f;
  float attackCooldown = 0.f;
  Vec2 jumpFxPos;
  float jumpFxTimer = -1.f;
  AnimState anim = AnimState::IDLE;

  int maxHp = 3;
  int currentHp = 3;
  float iframeTimer = 0.f;
  int jumpsLeft = 0;
  int orbs = 0;
  bool hasDoubleJump = false;
  bool isDead = false;
  bool pendingDeath = false;
  float deathTimer = 0.f;
  int deathFrame = 0;
  float deathFrameTimer = 0.f;
  bool attackHitDone = false;
  Vec2 attackFxPos = {};
  bool attackFxFacing = true;
  float rollGraceTimer = 0.f;
  int lostHeartSlot = -1;
  float lostHeartTimer = -1.f;
  int lostHeartFrame = 0;
  float lostHeartFrameTimer = 0.f;
  float hitAnimTimer = -1.f;
  float facingCoyoteTimer = 0.f;
  bool lastFacingRight = true;
  int gainedHeartSlot = -1;
  float gainedHeartTimer = -1.f;
  int gainedHeartFrame = 4;
  float gainedHeartFrameTimer = 0.f;
};

struct PhysicsField;

void updatePlayer(Player &p, const Input &input, const Room &room,
                  const PhysicsField &phys, float dt);
void damagePlayer(Player &p, int amount, float knockbackX);

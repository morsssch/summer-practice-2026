#pragma once

constexpr int MAX_GRASS = 512;

struct GrassSprite {
    float rootX     = 0.f;
    float rootY     = 0.f;
    int   tileCol   = 0;
    int   tileRow   = 0;
    float angle     = 0.f;
    float angleVel  = 0.f;
    float restAngle = 0.f;
    bool  active    = false;
};

struct GrassField {
    GrassSprite sprites[MAX_GRASS];
    int         count = 0;
    float       time  = 0.f;
};

struct Player;
struct Room;
struct EnemyField;

void initGrass(GrassField& g, const Room& room);
void updateGrass(GrassField& g, const Player& player, const EnemyField& ef, float dt);

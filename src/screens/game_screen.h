#pragma once
#include "core/types.h"
#include "core/input.h"
#include "core/app_state.h"
#include "game/room.h"
#include "game/player.h"
#include "game/grass.h"
#include "game/enemy.h"
#include "game/spore.h"
#include "game/item_drop.h"

struct Renderer;

constexpr int ARTIFACT_COUNT = 5;
constexpr int ART_GLOW  = 0;
constexpr int ART_WIND  = 1;
constexpr int ART_MOTH  = 2;
constexpr int ART_SPIKE = 3;
constexpr int ART_SPORE = 4;

constexpr int   DEATH_ITEMS   = 2;
constexpr float DEATH_ITEM_X  = 240.f;
constexpr float DEATH_ITEM_Y0 = 97.f;
constexpr float DEATH_ITEM_DY = 26.f;
constexpr float DEATH_ITEM_W  = 160.f;
constexpr float DEATH_ITEM_H  = 22.f;

struct DeathScreenState {
    int  sel        = 0;
    bool kb         = false;
    bool isHovering = false;
};

struct GameState {
    Player          player;
    Camera          camera;
    GrassField      grass;
    EnemyField      enemies;
    SporeField      spores;
    ItemField       items;
    const Room*     room = nullptr;
    bool            artifacts[ARTIFACT_COUNT] = {};
    bool            deathScreen = false;
    DeathScreenState deathSS;
};

void     initGameState(GameState& s);
AppState updateGame(GameState& s, const Input& input, float dt);
void     drawGame(Renderer& r, const GameState& s);

void updateCamera(Camera& cam, const Player& player, const Room& room, float dt);
void drawPlayer(Renderer& r, const Player& p);

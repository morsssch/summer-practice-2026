#pragma once
#include "core/types.h"
#include "core/input.h"
#include "core/app_state.h"
#include "game/room.h"
#include "game/player.h"

struct Renderer;

struct GameState {
    Player      player;
    Camera      camera;
    const Room* room = nullptr;
};

void     initGameState(GameState& s);
AppState updateGame(GameState& s, const Input& input, float dt);
void     drawGame(Renderer& r, const GameState& s);

void updateCamera(Camera& cam, const Player& player, const Room& room, float dt);
void drawPlayer(Renderer& r, const Player& p);

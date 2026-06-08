#pragma once

#include "core/app_state.h"
#include "core/input.h"
#include "game/state/game_state.h"

struct Renderer;

AppState updateGame(GameState &s, const Input &input, float dt);
void drawGame(Renderer &r, const GameState &s);

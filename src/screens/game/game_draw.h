#pragma once

#include "render/core/renderer.h"

struct GameState;

void drawStaticWorld(Renderer &r, const GameState &s);
void drawEntities(Renderer &r, const GameState &s);
void drawLightOccluders(Renderer &r, const GameState &s);
void drawDeathOverlay(Renderer &r, const GameState &s);

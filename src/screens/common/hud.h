#pragma once
#include "game/actor/player.h"
#include "render/core/renderer.h"

void drawHUD(Renderer &r, const Player &p, const bool *artifacts,
             int artifactCount);

#pragma once
#include "render/renderer.h"
#include "game/player.h"

void drawHUD(Renderer& r, const Player& p, const bool* artifacts, int artifactCount);

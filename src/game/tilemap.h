#pragma once

#include <cstdint>
#include "render/renderer.h"

void drawTilemap(Renderer& r, const uint8_t tiles[22][40]);
bool isSolid(const uint8_t tiles[22][40], float worldX, float worldY);
bool isPlatform(const uint8_t tiles[22][40], float worldX, float worldY);

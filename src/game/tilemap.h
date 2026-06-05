#pragma once

#include <cstdint>
#include "render/renderer.h"

void drawTilemap(Renderer& r, const uint8_t tiles[17][30]);
bool isSolid(const uint8_t tiles[17][30], float worldX, float worldY);
bool isPlatform(const uint8_t tiles[17][30], float worldX, float worldY);

#pragma once

#include "room.h"
#include "render/renderer.h"

void drawTilemap(Renderer& r, const Room& room);
bool isSolid(const Room& room, float worldX, float worldY);
bool isPlatform(const Room& room, float worldX, float worldY);

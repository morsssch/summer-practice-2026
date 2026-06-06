#pragma once

#include "core/types.h"
#include "game/room.h"

struct Renderer;
struct Player;

void updateCamera(Camera& cam, const Player& player, const Room& room, float dt);
void drawPlayer(Renderer& r, const Player& p);

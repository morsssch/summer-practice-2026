#include "game_screen.h"
#include "render/renderer.h"
#include "render/sprites.h"
#include "game/player.h"
#include "game/tilemap.h"
#include "levels/screen_01.h"
#include <cmath>
#include <algorithm>

void initGameState(GameState& s) {
    s.player     = Player{};
    s.player.pos = { 80.f, 498.f };
    s.camera     = Camera{};
    s.room       = &SCREEN_01;

    float viewW  = WINDOW_W / s.camera.zoom;
    float viewH  = WINDOW_H / s.camera.zoom;
    s.camera.x   = std::clamp(s.player.pos.x + 8 - viewW / 2.f, 0.f, s.room->w * TILE_SIZE - viewW);
    s.camera.y   = std::clamp(s.player.pos.y + 8 - viewH / 2.f, 0.f, s.room->h * TILE_SIZE - viewH);
}

AppState updateGame(GameState& s, const Input& input, float dt) {
    if (input.escapePressed) return AppState::PAUSE;
    updatePlayer(s.player, input, *s.room, dt);
    updateCamera(s.camera, s.player, *s.room, dt);
    return AppState::GAME;
}

void drawGame(Renderer& r, const GameState& s) {
    setUIView(r);
    drawBackground(r);
    setWorldView(r, s.camera);
    drawTilemap(r, *s.room);
    drawPlayer(r, s.player);
}

void updateCamera(Camera& cam, const Player& player, const Room& room, float dt)
{
    float viewW = WINDOW_W / cam.zoom;
    float viewH = WINDOW_H / cam.zoom;

    float targetX = player.pos.x + 8 - viewW / 2.f;
    float targetY = player.pos.y + 8 - viewH / 2.f;

    cam.x += (targetX - cam.x) * (1.f - std::exp(-8.f * dt));
    cam.y += (targetY - cam.y) * (1.f - std::exp(-8.f * dt));

    cam.x = std::clamp(cam.x, 0.f, room.w * TILE_SIZE - viewW);
    cam.y = std::clamp(cam.y, 0.f, room.h * TILE_SIZE - viewH);
}

void drawPlayer(Renderer& r, const Player& p)
{
    if (p.jumpFxTimer >= 0.f) {
        int frame = (int)(p.jumpFxTimer / 0.07f);
        if (frame < 4)
            drawSprite(r, HERO_JUMP_FX[frame], p.jumpFxPos.x, p.jumpFxPos.y);
    }

    if (p.anim == AnimState::ATTACK) {
        float drawX = p.facingRight ? p.pos.x : p.pos.x - 16;
        drawSprite(r, HERO_ATTACK[p.animFrame], drawX, p.pos.y, !p.facingRight);
        return;
    }

    const SpriteFrame* anim;
    switch (p.anim) {
        case AnimState::RUN:  anim = HERO_RUN;  break;
        case AnimState::JUMP: anim = HERO_JUMP; break;
        case AnimState::FALL: anim = HERO_FALL; break;
        case AnimState::LAND: anim = HERO_LAND; break;
        case AnimState::ROLL: anim = HERO_ROLL; break;
        default:              anim = HERO_IDLE; break;
    }
    drawSprite(r, anim[p.animFrame], p.pos.x, p.pos.y, !p.facingRight);
}

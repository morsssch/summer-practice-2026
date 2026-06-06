#include "game_screen.h"
#include "render/renderer.h"
#include "render/sprites.h"
#include "game/player.h"
#include "game/grass.h"
#include "game/tilemap.h"
#include "levels/screen_01.h"
#include <cmath>
#include <algorithm>

static const sf::Color SUN_LIT = {255, 240, 200, 255};

static float castRay(const Room& room, float ox, float oy, float dx, float dy, float maxDist) {
    const float STEP = 4.f;
    for (float t = STEP; t < maxDist; t += STEP) {
        float px = ox + dx * t, py = oy + dy * t;
        if (isSolid(room, px, py) || isPlatform(room, px, py))
            return t;
    }
    return maxDist;
}

static void buildLightPolygon(const Room& room, float wx, float wy, float radius,
                               const Camera& cam, float* out, int& count) {
    const int RAYS = 90;
    count = 0;
    for (int i = 0; i <= RAYS; i++) {
        float a  = (float)i / RAYS * 2.f * 3.14159265f;
        float dx = std::cos(a), dy = std::sin(a);
        float t  = castRay(room, wx, wy, dx, dy, radius);
        float hx = wx + dx * t;
        float hy = wy + dy * t;
        out[count * 2 + 0] = (hx - cam.x) * cam.zoom / LIGHT_SCALE;
        out[count * 2 + 1] = (float)LIGHT_H - (hy - cam.y) * cam.zoom / LIGHT_SCALE;
        count++;
    }
}

void initGameState(GameState& s) {
    s.player     = Player{};
    s.player.pos = { 80.f, 498.f };
    s.camera     = Camera{};
    s.room       = &SCREEN_01;

    float viewW  = WINDOW_W / s.camera.zoom;
    float viewH  = WINDOW_H / s.camera.zoom;
    s.camera.x   = std::clamp(s.player.pos.x + 8 - viewW / 2.f, 0.f, s.room->w * TILE_SIZE - viewW);
    s.camera.y   = std::clamp(s.player.pos.y + 8 - viewH / 2.f, 0.f, s.room->h * TILE_SIZE - viewH);
    initGrass(s.grass, *s.room);
}

AppState updateGame(GameState& s, const Input& input, float dt) {
    if (input.escapePressed) return AppState::PAUSE;
    updatePlayer(s.player, input, *s.room, dt);
    updateCamera(s.camera, s.player, *s.room, dt);
    updateGrass(s.grass, s.player, dt);
    return AppState::GAME;
}

void drawGame(Renderer& r, const GameState& s) {
    setUIView(r);
    drawBackground(r);
    setWorldView(r, s.camera);
    drawTilemap(r, *s.room);
    for (int i = 0; i < s.grass.count; i++) {
        const GrassSprite& g = s.grass.sprites[i];
        if (g.active) drawGrassSprite(r, g.rootX, g.rootY, g.tileCol, g.tileRow, g.angle);
    }
    drawPlayer(r, s.player);

    beginLightMap(r, SUN_LIT);
    setUIView(r);
    applyLightMap(r);
    drawScanlines(r);
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

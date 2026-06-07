#include "game_screen.h"
#include "hud.h"
#include "render/renderer.h"
#include "render/sprites.h"
#include "game/player.h"
#include "game/grass.h"
#include "game/tilemap.h"
#include "game/enemy.h"
#include "game/combat.h"
#include "game/spore.h"
#include "game/torch.h"
#include "game/item_drop.h"
#include "levels/screen_01.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

static constexpr sf::Color SUN_LIT   = {255, 240, 200, 255};
static constexpr int       PUFF_SEGS = 8;

static PlayerAttackProps makeAttackProps(const bool* artifacts) {
    return { artifacts[ART_SPIKE] ? 24.f : 16.f, 80.f };
}

static const SpriteFrame& getEnemyFrame(int type, EnemyAnim anim, int frame) {
    if ((EnemyType)type == EnemyType::SLIME) {
        switch (anim) {
            case EnemyAnim::IDLE:   return SLIME_IDLE[frame];
            case EnemyAnim::WALK:   return SLIME_JUMP[frame];
            case EnemyAnim::HIT:    return SLIME_HIT[frame];
            case EnemyAnim::DEATH:  return SLIME_DEATH[frame];
            default:                return SLIME_IDLE[0];
        }
    }
    switch (anim) {
        case EnemyAnim::ATTACK: return MUSHROOM_ATTACK[frame];
        case EnemyAnim::HIT:    return MUSHROOM_HIT[frame];
        case EnemyAnim::DEATH:  return MUSHROOM_DEATH[frame];
        default:                return MUSHROOM_WALK[frame];
    }
}

static void drawSporeCloud(Renderer& r, const SporeCloud& c) {
    float lifeFrac = c.lifetime / SPORE_VISIBLE;

    sf::VertexArray va(sf::PrimitiveType::Triangles,
                       SporeCloud::PUFF_COUNT * PUFF_SEGS * 3);
    int idx = 0;

    for (int i = 0; i < SporeCloud::PUFF_COUNT; i++) {
        const SporeCloud::Puff& puff = c.puffs[i];

        float px  = c.x + puff.ox + puff.dx * c.age;
        float py  = c.y + puff.oy + puff.dy * c.age;
        float rad = puff.radius * (0.3f + 0.7f * std::min(1.f, c.age * 1.8f));

        uint8_t a     = (uint8_t)(puff.alpha * lifeFrac);
        sf::Color inner(160, 40, 220, a);
        sf::Color outer(90,   0, 150, 0);

        for (int k = 0; k < PUFF_SEGS; k++) {
            float ang0 = k       * (PI2 / PUFF_SEGS);
            float ang1 = (k + 1) * (PI2 / PUFF_SEGS);
            va[idx++] = {{px,                              py                             }, inner};
            va[idx++] = {{px + std::cos(ang0) * rad,      py + std::sin(ang0) * rad      }, outer};
            va[idx++] = {{px + std::cos(ang1) * rad,      py + std::sin(ang1) * rad      }, outer};
        }
    }

    r.window->draw(va, sf::BlendAdd);
}

void initGameState(GameState& s) {
    s.player      = Player{};
    s.player.pos  = { 80.f, 496.f };
    s.camera      = Camera{};
    s.room        = &SCREEN_01;
    s.deathScreen = false;
    s.deathSS     = {};
    s.spores      = {};
    s.items       = {};
    for (int i = 0; i < ARTIFACT_COUNT; i++) s.artifacts[i] = false;
    s.artifacts[ART_WIND] = true;
    initEnemies(s.enemies, *s.room);

    float viewW  = WINDOW_W / s.camera.zoom;
    float viewH  = WINDOW_H / s.camera.zoom;
    s.camera.x   = std::clamp(s.player.pos.x + 8 - viewW / 2.f, 0.f, s.room->w * TILE_SIZE - viewW);
    s.camera.y   = std::clamp(s.player.pos.y + 8 - viewH / 2.f, 0.f, s.room->h * TILE_SIZE - viewH);
    initGrass(s.grass, *s.room);
    initTorches(s.torches, *s.room);
}

static const char* DEATH_LABELS[DEATH_ITEMS] = { "Начать заново", "В меню" };

AppState updateGame(GameState& s, const Input& input, float dt) {
    if (s.deathScreen) {
        DeathScreenState& ds = s.deathSS;
        if (input.upPressed || input.downPressed) ds.kb = true;
        if (input.mouseMoved)                     ds.kb = false;
        if (input.upPressed)   ds.sel = (ds.sel - 1 + DEATH_ITEMS) % DEATH_ITEMS;
        if (input.downPressed) ds.sel = (ds.sel + 1) % DEATH_ITEMS;

        ds.isHovering    = false;
        bool mouseSelect = false;
        for (int i = 0; i < DEATH_ITEMS; i++) {
            float x0 = DEATH_ITEM_X - DEATH_ITEM_W / 2.f;
            float y0 = DEATH_ITEM_Y0 + i * DEATH_ITEM_DY - DEATH_ITEM_H / 2.f;
            if (input.mouseX >= x0 && input.mouseX <= x0 + DEATH_ITEM_W &&
                input.mouseY >= y0 && input.mouseY <= y0 + DEATH_ITEM_H) {
                ds.isHovering = true;
                if (!ds.kb) ds.sel = i;
                if (input.mouseClicked) { ds.sel = i; mouseSelect = true; }
            }
        }

        if (input.confirmPressed || mouseSelect) {
            if (ds.sel == 0) initGameState(s);
            else             return AppState::MENU;
        }
        if (input.escapePressed) return AppState::MENU;
        return AppState::GAME;
    }

    if (input.escapePressed) return AppState::PAUSE;
    s.player.hasDoubleJump = s.artifacts[ART_WIND];
    updatePlayer(s.player, input, *s.room, dt);

    if (s.player.isDead && s.player.deathTimer <= 0.f) {
        s.deathScreen = true;
        s.deathSS     = {};
    }

    updateEnemies(s.enemies, *s.room, s.player.pos, dt);
    resolveAttack(s.player, s.enemies, makeAttackProps(s.artifacts));
    resolveEnemyContact(s.enemies, s.player);
    resolveEnemyAttack(s.enemies, s.player);
    for (int i = 0; i < s.enemies.count; i++) {
        Enemy& e = s.enemies.enemies[i];
        if (!e.active || e.dying || e.anim != EnemyAnim::ATTACK || e.attackFxSpawned) continue;
        if (ENEMY_TEMPLATES[e.type].attackEffect == AttackEffect::SPORE_CLOUD) {
            spawnSporeCloud(s.spores, e.pos.x + 8.f, e.pos.y);
        }
        e.attackFxSpawned = true;
    }
    updateItemDrops(s.items, dt);
    updateSpores(s.spores, s.player, dt);
    updateCamera(s.camera, s.player, *s.room, dt);
    updateGrass(s.grass, s.player, s.enemies, dt);
    updateTorches(s.torches, dt);
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
    for (int i = 0; i < s.torches.count; i++) {
        const TorchSprite& t = s.torches.sprites[i];
        drawTorch(r, TORCH_ANIM[t.animFrame], t.x, t.y);
    }
    for (int i = 0; i < s.enemies.count; i++) {
        const Enemy& e = s.enemies.enemies[i];
        if (!e.active) continue;
        if (e.iframeTimer > 0.f && (int)(e.iframeTimer * 10.f) % 2 == 0) continue;
        EnemyAnim drawAnim = e.dying ? EnemyAnim::DEATH : e.anim;
        const SpriteFrame& frame = getEnemyFrame(e.type, drawAnim, e.animFrame);
        if ((EnemyType)e.type == EnemyType::SLIME) {
            float drawY = e.pos.y - (frame.h - 16);
            drawSlime(r, frame, e.pos.x, drawY, e.facingRight);
        } else {
            drawMushroom(r, frame, e.pos.x, e.pos.y, !e.facingRight);
        }
    }
    for (int i = 0; i < MAX_SPORES; i++) {
        const SporeCloud& c = s.spores.clouds[i];
        if (!c.active || c.delay > 0.f) continue;
        drawSporeCloud(r, c);
    }
    drawPlayer(r, s.player);
    beginLightMap(r, SUN_LIT);
    LightOccluder occ[MAX_ENEMIES + 1];
    int occCount = 0;
    for (int i = 0; i < s.enemies.count; i++) {
        const Enemy& e = s.enemies.enemies[i];
        if (e.active && !e.dying)
            occ[occCount++] = { e.pos.x + 1.f, e.pos.y + 2.f, 14.f, 12.f };
    }
    if (!s.player.isDead)
        occ[occCount++] = { s.player.pos.x + 2.f, s.player.pos.y + 2.f, 12.f, 12.f };
    for (int i = 0; i < s.torches.count; i++) {
        const TorchSprite& torch = s.torches.sprites[i];
        float phase  = torch.x * 0.13f + torch.y * 0.07f;
        float speed  = 6.f + std::sin(torch.x * 0.21f) * 1.5f; 
        float radius = 55.f + std::sin(s.torches.time * speed + phase) * 5.f;
        addPointLight(r, *s.room, s.camera, torch.x + 4, torch.y + 8, radius, {255, 160, 60, 255}, occ, occCount);
    }
    setUIView(r);
    applyLightMap(r);
    drawScanlines(r);
    drawHUD(r, s.player, s.artifacts, ARTIFACT_COUNT);

    if (s.deathScreen) {
        drawRect(r, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, {0, 0, 0, 160});
        drawText(r, "ВЫ ПОГИБЛИ", WINDOW_W / 2.f, 62.f, 16, {220, 80, 80, 255});
        for (int i = 0; i < DEATH_ITEMS; i++) {
            float cy  = DEATH_ITEM_Y0 + i * DEATH_ITEM_DY;
            bool  sel = (i == s.deathSS.sel);
            if (sel)
                drawRect(r, DEATH_ITEM_X - DEATH_ITEM_W / 2.f, cy - DEATH_ITEM_H / 2.f,
                         DEATH_ITEM_W, DEATH_ITEM_H, {255, 255, 255, 25});
            sf::Color col = sel ? sf::Color{255, 220, 80, 255} : sf::Color{160, 160, 180, 255};
            drawText(r, DEATH_LABELS[i], DEATH_ITEM_X, cy, 16, col);
        }
    }
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
    if (p.isDead) {
        drawSprite(r, HERO_DEATH[p.deathFrame], p.pos.x, p.pos.y, !p.facingRight);
        return;
    }

    if (p.iframeTimer > 0.f) {
        if (p.hitAnimTimer >= 0.f && p.hitAnimTimer < 4 * 0.1f) {
            int hitFrame = std::min(3, (int)(p.hitAnimTimer / 0.1f));
            drawSprite(r, HERO_HIT[hitFrame], p.pos.x, p.pos.y, !p.facingRight);
            return;
        }
        if ((int)(p.iframeTimer * 8.f) % 2 == 0) return;
    }

    if (p.jumpFxTimer >= 0.f) {
        int frame = (int)(p.jumpFxTimer / 0.07f);
        if (frame < 4)
            drawSprite(r, HERO_JUMP_FX[frame], p.jumpFxPos.x, p.jumpFxPos.y);
    }

    if (p.anim == AnimState::ATTACK) {
        drawSprite(r, HERO_ATTACK_BODY[p.animFrame], p.pos.x, p.pos.y, !p.facingRight);
        drawSprite(r, HERO_ATTACK_FX[p.animFrame],  p.attackFxPos.x, p.attackFxPos.y, !p.attackFxFacing);
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

#include "screens/game/game_draw.h"
#include "game/state/game_state.h"
#include "game/world/tilemap.h"
#include "render/core/sprites.h"
#include "render/effects/lighting.h"
#include <algorithm>
#include <cmath>

static const char *DEATH_LABELS[DEATH_ITEMS] = {"Начать заново", "В меню"};

static void drawGrassLayer(Renderer &r, const GrassField &grass) {
  for (int i = 0; i < grass.count; i++) {
    const GrassSprite &g = grass.sprites[i];
    if (g.active)
      drawGrassSprite(r, g.rootX, g.rootY, g.tileCol, g.tileRow, g.angle);
  }
}

static void drawTorchLayer(Renderer &r, const TorchField &torches) {
  for (int i = 0; i < torches.count; i++) {
    const TorchSprite &t = torches.sprites[i];
    drawTorch(r, TORCH_ANIM[t.animFrame], t.x, t.y);
  }
}

static void drawTilemap(Renderer &r, const Room &room) {
  auto spriteFor = [](uint8_t t) -> const SpriteFrame * {
    switch (t) {
    case T_GRASS_TOP:
      return &TILE_GRASS_TOP;
    case T_GROUND:
      return &TILE_GROUND;
    case T_DARK_TOP:
      return &TILE_DARK_TOP;
    case T_DARK:
      return &TILE_DARK;
    case T_BROWN_TOP:
      return &TILE_BROWN_TOP;
    case T_BROWN:
      return &TILE_BROWN;
    case T_PLATFORM:
      return &TILE_GRASS_TOP;
    default:
      return nullptr;
    }
  };

  for (int y = 0; y < room.h; y++) {
    for (int x = 0; x < room.w; x++) {
      const SpriteFrame *sf = spriteFor(room.tiles[y][x]);
      if (sf)
        drawTile(r, *sf, x, y);
    }
  }
}

static void drawTraps(Renderer &r, const TrapSystem &t) {
  for (int i = 0; i < t.spikeCount; i++) {
    if (!t.spikes[i].active)
      continue;
    float x = t.spikes[i].tx * 16.f;
    float y = t.spikes[i].ty * 16.f;
    if (r.spikeLoaded) {
      sf::Sprite sprite(r.spikeTex);
      if (!t.spikes[i].ceil) {
        sprite.setScale({1.f, -1.f});
        sprite.setPosition({x, y + 16.f});
      } else {
        sprite.setPosition({x, y});
      }
      r.window->draw(sprite);
    } else {
      sf::Color c{220, 60, 60, 200};
      if (!t.spikes[i].ceil)
        drawRect(r, x + 2.f, y + 8.f, 12.f, 8.f, c);
      else
        drawRect(r, x + 2.f, y, 12.f, 8.f, c);
    }
  }

  for (int i = 0; i < t.plateCount; i++) {
    const PlateTile &pl = t.plates[i];
    if (!pl.active)
      continue;
    float x = pl.tx * 16.f;
    float y = pl.ty * 16.f;
    bool usePressed = pl.pressed || pl.locked;
    if (usePressed && r.platePressedLoaded) {
      sf::Sprite sprite(r.platePressedTex);
      sprite.setPosition({x, y + 12.f});
      r.window->draw(sprite);
    } else if (!usePressed && r.plateLoaded) {
      sf::Sprite sprite(r.plateTex);
      sprite.setPosition({x, y + 12.f});
      r.window->draw(sprite);
    } else {
      sf::Color c = usePressed ? sf::Color{200, 220, 80, 255}
                               : sf::Color{160, 120, 50, 255};
      drawRect(r, x + 1.f, y + 12.f, 14.f, 4.f, c);
    }
  }

  for (int i = 0; i < t.doorCount; i++) {
    const DoorTile &d = t.doors[i];
    if (!d.active || d.locked)
      continue;
    float x = d.tx * 16.f;
    float y = (d.ty - (DOOR_TILE_H - 1)) * 16.f;
    auto drawDoorSprite = [&](sf::Texture &tex, int frameX) {
      sf::Sprite sprite(tex);
      sprite.setTextureRect(sf::IntRect({frameX * 16, 0}, {16, 48}));
      if (d.flipX) {
        sprite.setScale({-1.f, 1.f});
        sprite.setPosition({x + 16.f, y});
      } else {
        sprite.setPosition({x, y});
      }
      r.window->draw(sprite);
    };
    if (d.openFrame == 0) {
      if (r.doorClosedLoaded)
        drawDoorSprite(r.doorClosedTex, d.idleFrame);
      else
        drawRect(r, x, y, 16.f, 48.f, {80, 60, 180, 220});
    } else {
      if (r.doorOpeningLoaded)
        drawDoorSprite(r.doorOpeningTex, d.openFrame);
      else {
        uint8_t a = (uint8_t)(220 - d.openFrame * 15);
        drawRect(r, x, y, 16.f, 48.f, {80, 60, 180, a});
      }
    }
  }
}

static void drawPhysicsTiles(Renderer &r, const PhysicsField &f) {
  for (int i = 0; i < f.count; i++) {
    const PhysicsTile &t = f.tiles[i];
    if (!t.active)
      continue;
    if (!r.stoneLoaded) {
      drawRect(r, t.pos.x, t.pos.y, 16.f, 16.f, {120, 100, 80, 255});
      continue;
    }
    sf::Sprite sprite(r.stoneTex);
    sprite.setPosition({t.pos.x, t.pos.y});
    r.window->draw(sprite);
  }
}

static const SpriteFrame &getEnemyFrame(int type, EnemyAnim anim, int frame) {
  if ((EnemyType)type == EnemyType::SLIME) {
    switch (anim) {
    case EnemyAnim::IDLE:
      return SLIME_IDLE[frame];
    case EnemyAnim::WALK:
      return SLIME_JUMP[frame];
    case EnemyAnim::HIT:
      return SLIME_HIT[frame];
    case EnemyAnim::DEATH:
      return SLIME_DEATH[frame];
    default:
      return SLIME_IDLE[0];
    }
  }
  switch (anim) {
  case EnemyAnim::ATTACK:
    return MUSHROOM_ATTACK[frame];
  case EnemyAnim::HIT:
    return MUSHROOM_HIT[frame];
  case EnemyAnim::DEATH:
    return MUSHROOM_DEATH[frame];
  default:
    return MUSHROOM_WALK[frame];
  }
}

static void drawEnemies(Renderer &r, const EnemyField &ef) {
  for (int i = 0; i < ef.count; i++) {
    const Enemy &e = ef.enemies[i];
    if (!e.active)
      continue;
    if (e.iframeTimer > 0.f && (int)(e.iframeTimer * 10.f) % 2 == 0)
      continue;
    EnemyAnim drawAnim = e.dying ? EnemyAnim::DEATH : e.anim;
    const SpriteFrame &frame = getEnemyFrame(e.type, drawAnim, e.animFrame);
    if ((EnemyType)e.type == EnemyType::SLIME) {
      float drawY = e.pos.y - (frame.h - 16);
      drawSlime(r, frame, e.pos.x, drawY, e.facingRight);
    } else {
      drawMushroom(r, frame, e.pos.x, e.pos.y, !e.facingRight);
    }
  }
}

static void drawItems(Renderer &r, const ItemField &f) {
  for (int i = 0; i < MAX_ITEM_DROPS; i++) {
    const ItemDrop &d = f.drops[i];
    if (!d.active)
      continue;
    bool blink = d.lifetime < 3.f && (int)(d.lifetime * 8.f) % 2 == 0;
    uint8_t alpha = blink ? 80 : 255;
    if (d.collected) {
      if (d.collectFrame < 5)
        drawOrbCollect(r, ORB_COLLECTED[d.collectFrame], d.pos.x, d.pos.y);
    } else if (d.type == ITEM_ORB) {
      drawOrbDrop(r, ORB_ANIM[d.animFrame], d.pos.x, d.pos.y, alpha);
    } else if (d.type == ITEM_POTION) {
      float bob = std::sin((12.f - d.lifetime) * 3.f) * 1.5f;
      drawPotion(r, d.pos.x, d.pos.y + bob, alpha);
    }
  }
}

static void drawPlayer(Renderer &r, const Player &p) {
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
    if ((int)(p.iframeTimer * 8.f) % 2 == 0)
      return;
  }
  if (p.jumpFxTimer >= 0.f) {
    int frame = (int)(p.jumpFxTimer / 0.07f);
    if (frame < 4)
      drawSprite(r, HERO_JUMP_FX[frame], p.jumpFxPos.x, p.jumpFxPos.y);
  }
  if (p.anim == AnimState::ATTACK) {
    drawSprite(r, HERO_ATTACK_BODY[p.animFrame], p.pos.x, p.pos.y,
               !p.facingRight);
    drawSprite(r, HERO_ATTACK_FX[p.animFrame], p.attackFxPos.x, p.attackFxPos.y,
               !p.attackFxFacing);
    return;
  }
  const SpriteFrame *anim;
  switch (p.anim) {
  case AnimState::RUN:
    anim = HERO_RUN;
    break;
  case AnimState::JUMP:
    anim = HERO_JUMP;
    break;
  case AnimState::FALL:
    anim = HERO_FALL;
    break;
  case AnimState::LAND:
    anim = HERO_LAND;
    break;
  case AnimState::ROLL:
    anim = HERO_ROLL;
    break;
  default:
    anim = HERO_IDLE;
    break;
  }
  drawSprite(r, anim[p.animFrame], p.pos.x, p.pos.y, !p.facingRight);
}

static void drawSpores(Renderer &r, const SporeField &sf) {
  for (int i = 0; i < MAX_SPORES; i++) {
    const SporeCloud &c = sf.clouds[i];
    if (!c.active || c.delay > 0.f)
      continue;
    float lifeFrac = c.lifetime / SPORE_VISIBLE;
    sf::VertexArray va(sf::PrimitiveType::Triangles,
                       SporeCloud::PUFF_COUNT * 8 * 3);
    int idx = 0;
    for (int i = 0; i < SporeCloud::PUFF_COUNT; i++) {
      const SporeCloud::Puff &puff = c.puffs[i];
      float px = c.x + puff.ox + puff.dx * c.age;
      float py = c.y + puff.oy + puff.dy * c.age;
      float rad = puff.radius * (0.3f + 0.7f * std::min(1.f, c.age * 1.8f));
      uint8_t a = (uint8_t)(puff.alpha * lifeFrac);
      sf::Color inner(160, 40, 220, a);
      sf::Color outer(90, 0, 150, 0);
      for (int k = 0; k < 8; k++) {
        float ang0 = k * (PI2 / 8);
        float ang1 = (k + 1) * (PI2 / 8);
        va[idx++] = {{px, py}, inner};
        va[idx++] = {{px + std::cos(ang0) * rad, py + std::sin(ang0) * rad},
                     outer};
        va[idx++] = {{px + std::cos(ang1) * rad, py + std::sin(ang1) * rad},
                     outer};
      }
    }
    r.window->draw(va, sf::BlendAdd);
  }
}

void drawStaticWorld(Renderer &r, const GameState &s) {
  drawTilemap(r, s.room);
  drawTraps(r, s.traps);
  drawPhysicsTiles(r, s.physics);
  drawGrassLayer(r, s.grass);
  drawTorchLayer(r, s.torches);
}

void drawEntities(Renderer &r, const GameState &s) {
  drawItems(r, s.items);
  drawEnemies(r, s.enemies);
  drawSpores(r, s.spores);
  drawPlayer(r, s.player);
}

void drawLightOccluders(Renderer &r, const GameState &s) {
  LightOccluder occ[MAX_ENEMIES + 1 + MAX_PHYS_TILES];
  int occCount = 0;
  for (int i = 0; i < s.enemies.count; i++) {
    const Enemy &e = s.enemies.enemies[i];
    if (e.active && !e.dying)
      occ[occCount++] = {e.pos.x + 1.f, e.pos.y + 2.f, 14.f, 12.f};
  }
  if (!s.player.isDead)
    occ[occCount++] = {s.player.pos.x + 2.f, s.player.pos.y + 2.f, 12.f, 12.f};
  for (int i = 0; i < s.physics.count; i++) {
    const PhysicsTile &b = s.physics.tiles[i];
    if (b.active)
      occ[occCount++] = {b.pos.x, b.pos.y, 16.f, 16.f};
  }
  for (int i = 0; i < s.torches.count; i++) {
    const TorchSprite &torch = s.torches.sprites[i];
    float phase = torch.x * 0.13f + torch.y * 0.07f;
    float speed = 6.f + std::sin(torch.x * 0.21f) * 1.5f;
    float radius = 55.f + std::sin(s.torches.time * speed + phase) * 5.f;
    addPointLight(r, s.room, s.camera, torch.x + 4, torch.y + 8, radius,
                  {255, 160, 60, 255}, occ, occCount);
  }
}

void drawDeathOverlay(Renderer &r, const GameState &s) {
  drawRect(r, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, {0, 0, 0, 160});
  drawText(r, "ВЫ ПОГИБЛИ", WINDOW_W / 2.f, 62.f, 16, {220, 80, 80, 255});
  for (int i = 0; i < DEATH_ITEMS; i++) {
    float cy = DEATH_ITEM_Y0 + i * DEATH_ITEM_DY;
    bool sel = (i == s.deathSS.sel);
    if (sel)
      drawRect(r, DEATH_ITEM_X - DEATH_ITEM_W / 2.f, cy - DEATH_ITEM_H / 2.f,
               DEATH_ITEM_W, DEATH_ITEM_H, {255, 255, 255, 25});
    sf::Color col =
        sel ? sf::Color{255, 220, 80, 255} : sf::Color{160, 160, 180, 255};
    drawText(r, DEATH_LABELS[i], DEATH_ITEM_X, cy, 16, col);
  }
}

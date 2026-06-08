#include "renderer.h"

#include <SFML/Graphics.hpp>
#include <cmath>

static bool loadTex(sf::Texture &tex, const char *path) {
  return tex.loadFromFile(path);
}

bool initRenderer(Renderer &r, sf::RenderWindow &window) {
  r.window = &window;
  if (!loadTex(r.tilesetTex, "assets/tileset.png"))
    return false;
  if (!loadTex(r.heroTex, "assets/hero.png"))
    return false;
  if (!loadTex(r.bgTex, "assets/background.png"))
    return false;
  r.fontLoaded = r.font.openFromFile("assets/font.ttf");
  if (r.fontLoaded)
    r.font.setSmooth(false);
  r.grassLoaded = loadTex(r.grassTex, "assets/grass.png");
  r.mushroomLoaded = loadTex(r.mushroomTex, "assets/mushroom_enemy.png");
  r.slimeLoaded = loadTex(r.slimeTex, "assets/slime_enemy.png");
  r.torchLoaded = loadTex(r.torchTex, "assets/torch.png");
  r.spikeLoaded = loadTex(r.spikeTex, "assets/spikes.png");
  r.plateLoaded = loadTex(r.plateTex, "assets/plate.png");
  r.platePressedLoaded = loadTex(r.platePressedTex, "assets/plate_pressed.png");
  r.doorClosedLoaded = loadTex(r.doorClosedTex, "assets/door_closed.png");
  r.doorOpeningLoaded = loadTex(r.doorOpeningTex, "assets/door_opening.png");
  r.stoneLoaded = loadTex(r.stoneTex, "assets/stone.png");
  r.orbDropLoaded = loadTex(r.orbDropTex, "assets/orb_drop.png");
  r.orbCollectedLoaded = loadTex(r.orbCollectedTex, "assets/orb_collected.png");
  r.potionLoaded = loadTex(r.potionTex, "assets/health_potion.png");
  r.hudHeartsLoaded = loadTex(r.hudHeartFullTex, "assets/hearts_hud.png") &&
                      loadTex(r.hudHeartEmptyTex, "assets/no_hearts_hud.png");
  r.hudHeroIconLoaded = loadTex(r.hudHeroIconTex, "assets/lifes_icon.png");
  r.hudLostHeartLoaded =
      loadTex(r.hudLostHeartTex, "assets/lost_hearts_anim_strip_5.png");
  (void)r.lightMap.resize({(unsigned)LIGHT_W, (unsigned)LIGHT_H});
  r.lightMap.setSmooth(false);
  return true;
}

static sf::FloatRect calcLetterboxViewport(const sf::RenderWindow &window) {
  float targetRatio = (float)WINDOW_W / (float)WINDOW_H;
  float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
  if (windowRatio > targetRatio) {
    float w = targetRatio / windowRatio;
    return sf::FloatRect({(1.f - w) / 2.f, 0.f}, {w, 1.f});
  } else {
    float h = windowRatio / targetRatio;
    return sf::FloatRect({0.f, (1.f - h) / 2.f}, {1.f, h});
  }
}

void applyLetterbox(Renderer &r) {
  r.viewport = calcLetterboxViewport(*r.window);
}

void setWorldView(Renderer &r, const Camera &cam) {
  sf::View view(sf::FloatRect({cam.x, cam.y},
                              {WINDOW_W / cam.zoom, WINDOW_H / cam.zoom}));
  view.setViewport(r.viewport);
  r.window->setView(view);
}

void setUIView(Renderer &r) {
  sf::View view(sf::FloatRect({0, 0}, {(float)WINDOW_W, (float)WINDOW_H}));
  view.setViewport(r.viewport);
  r.window->setView(view);
}

void drawFrom(Renderer &r, const sf::Texture &tex, const SpriteFrame &frame,
              float x, float y, bool flipX, uint8_t alpha) {
  sf::Sprite sprite(tex);
  sprite.setTextureRect(sf::IntRect({frame.x, frame.y}, {frame.w, frame.h}));
  if (alpha != 255)
    sprite.setColor(sf::Color(255, 255, 255, alpha));
  if (flipX) {
    sprite.setScale({-1.f, 1.f});
    sprite.setPosition({x + frame.w, y});
  } else {
    sprite.setPosition({x, y});
  }
  r.window->draw(sprite);
}

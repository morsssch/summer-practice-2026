#include "render/core/renderer.h"

void drawOrbDrop(Renderer &r, const SpriteFrame &frame, float x, float y,
                 uint8_t alpha) {
  if (!r.orbDropLoaded) {
    drawRect(r, x, y, 8.f, 8.f, {255, 200, 50, alpha});
    return;
  }
  drawFrom(r, r.orbDropTex, frame, x, y, false, alpha);
}

void drawOrbCollect(Renderer &r, const SpriteFrame &frame, float x, float y) {
  if (!r.orbCollectedLoaded)
    return;
  drawFrom(r, r.orbCollectedTex, frame, x, y);
}

void drawPotion(Renderer &r, float x, float y, uint8_t alpha) {
  if (!r.potionLoaded) {
    drawRect(r, x, y, 8.f, 10.f, {80, 220, 80, alpha});
    return;
  }
  sf::Sprite sprite(r.potionTex);
  sprite.setColor(sf::Color(255, 255, 255, alpha));
  sprite.setPosition({x, y});
  r.window->draw(sprite);
}

void drawOrbHudIcon(Renderer &r, float x, float y) {
  if (!r.orbDropLoaded) {
    drawRect(r, x, y, 12.f, 12.f, {255, 200, 50, 220});
    return;
  }
  sf::Sprite sprite(r.orbDropTex);
  sprite.setTextureRect(sf::IntRect({0, 0}, {8, 8}));
  sprite.setScale({1.5f, 1.5f});
  sprite.setPosition({x, y});
  r.window->draw(sprite);
}

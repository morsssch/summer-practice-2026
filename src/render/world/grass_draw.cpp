#include "render/core/renderer.h"

void drawGrassSprite(Renderer &r, float x, float y, int tileCol, int tileRow,
                     float angleDeg) {
  if (!r.grassLoaded)
    return;
  sf::Sprite sprite(r.grassTex);
  sprite.setTextureRect(sf::IntRect({tileCol * 16, tileRow * 16}, {16, 16}));
  sprite.setOrigin({8.f, 16.f});
  sprite.setPosition({x, y});
  sprite.setRotation(sf::degrees(angleDeg));
  r.window->draw(sprite);
}

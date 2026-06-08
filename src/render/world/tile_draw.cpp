#include "render/core/renderer.h"

void drawTile(Renderer &r, const SpriteFrame &frame, int tileX, int tileY) {
  sf::Sprite sprite(r.tilesetTex);
  sprite.setTextureRect(sf::IntRect({frame.x, frame.y}, {frame.w, frame.h}));
  sprite.setPosition({(float)(tileX * 16), (float)(tileY * 16)});
  r.window->draw(sprite);
}

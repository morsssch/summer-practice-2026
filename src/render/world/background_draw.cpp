#include "render/core/renderer.h"

void drawBackground(Renderer &r) {
  sf::Sprite sprite(r.bgTex);
  sprite.setScale({(float)WINDOW_W / 240.f, (float)WINDOW_H / 160.f});
  r.window->draw(sprite);
}

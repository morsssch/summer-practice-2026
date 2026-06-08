#include "render/core/renderer.h"

void drawUISprite(Renderer &r, sf::Texture &tex, float x, float y) {
  sf::Sprite sprite(tex);
  sprite.setPosition({x, y});
  r.window->draw(sprite);
}

void drawLostHeartFrame(Renderer &r, int frame, float x, float y) {
  if (!r.hudLostHeartLoaded || frame < 0 || frame >= 5)
    return;
  sf::Sprite sprite(r.hudLostHeartTex);
  sprite.setTextureRect(sf::IntRect({frame * 16, 0}, {16, 16}));
  sprite.setPosition({x, y});
  r.window->draw(sprite);
}

void drawScanlines(Renderer &r) {
  const int step = 2;
  const uint8_t alpha = 15;
  sf::VertexArray lines(sf::PrimitiveType::Triangles, (WINDOW_H / step) * 6);
  int idx = 0;
  for (int y = 0; y < WINDOW_H; y += step) {
    sf::Color c(52, 10, 44, alpha);
    lines[idx++] = {{0.f, (float)y}, c};
    lines[idx++] = {{(float)WINDOW_W, (float)y}, c};
    lines[idx++] = {{(float)WINDOW_W, (float)(y + 1)}, c};
    lines[idx++] = {{0.f, (float)y}, c};
    lines[idx++] = {{(float)WINDOW_W, (float)(y + 1)}, c};
    lines[idx++] = {{0.f, (float)(y + 1)}, c};
  }
  r.window->draw(lines);
}

#include "render/core/renderer.h"
#include <cmath>

void drawRect(Renderer &r, float x, float y, float w, float h,
              sf::Color color) {
  sf::RectangleShape rect({w, h});
  rect.setPosition({x, y});
  rect.setFillColor(color);
  r.window->draw(rect);
}

void drawText(Renderer &r, const std::string &str, float cx, float cy,
              unsigned int size, sf::Color color) {
  if (!r.fontLoaded)
    return;
  sf::Text text(r.font, sf::String::fromUtf8(str.begin(), str.end()), size);
  text.setFillColor(color);
  auto bounds = text.getLocalBounds();
  float tlx = std::round(cx - (bounds.position.x + bounds.size.x / 2.f));
  float tly = std::round(cy - (bounds.position.y + bounds.size.y / 2.f));
  text.setPosition({tlx, tly});
  r.window->draw(text);
}

void drawTextLeft(Renderer &r, const std::string &str, float x, float cy,
                  unsigned int size, sf::Color color) {
  if (!r.fontLoaded)
    return;
  sf::Text text(r.font, sf::String::fromUtf8(str.begin(), str.end()), size);
  text.setFillColor(color);
  auto bounds = text.getLocalBounds();
  float tly = std::round(cy - (bounds.position.y + bounds.size.y / 2.f));
  text.setPosition({std::round(x - bounds.position.x), tly});
  r.window->draw(text);
}

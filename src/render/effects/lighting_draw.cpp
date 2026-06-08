#include "core/lighting.h"
#include "render/effects/lighting.h"

void beginLightMap(Renderer &r, sf::Color ambient) {
  r.lightMap.clear(ambient);
}

void drawLightQuad(Renderer &r, float x0, float y0, float x1, float y1,
                   float x2, float y2, float x3, float y3, sf::Color nearColor,
                   sf::Color farColor) {
  sf::VertexArray quad(sf::PrimitiveType::TriangleFan, 4);
  quad[0] = {{x0, y0}, nearColor};
  quad[1] = {{x1, y1}, nearColor};
  quad[2] = {{x2, y2}, farColor};
  quad[3] = {{x3, y3}, farColor};
  r.lightMap.draw(quad);
}

void drawLightPolygon(Renderer &r, float cx, float cy, const float *xys,
                      int count, sf::Color inner) {
  sf::VertexArray fan(sf::PrimitiveType::TriangleFan, count + 1);
  fan[0].position = {cx, cy};
  fan[0].color = inner;
  for (int i = 0; i < count; i++) {
    fan[i + 1].position = {xys[i * 2], xys[i * 2 + 1]};
    fan[i + 1].color = sf::Color(inner.r, inner.g, inner.b, 0);
  }
  r.lightMap.draw(fan);
}

void applyLightMap(Renderer &r) {
  r.lightMap.display();
  sf::Sprite overlay(r.lightMap.getTexture());
  overlay.setScale({LIGHT_SCALE, -LIGHT_SCALE});
  overlay.setPosition({0.f, (float)WINDOW_H});
  r.window->draw(overlay, sf::BlendMultiply);
}

void addPointLight(Renderer &r, const Room &room, const Camera &cam, float wx,
                   float wy, float radius, sf::Color inner,
                   const LightOccluder *occ, int occCount) {
  constexpr int MAXP = 512;
  float worldPts[MAXP * 2];
  float lm[MAXP * 2];
  int count = 0;
  buildLightPolygon(room, wx, wy, radius, cam, worldPts, count, occ, occCount);

  for (int i = 0; i < count; i++) {
    lm[i * 2 + 0] = (worldPts[i * 2 + 0] - cam.x) * cam.zoom / LIGHT_SCALE;
    lm[i * 2 + 1] =
        (float)LIGHT_H - (worldPts[i * 2 + 1] - cam.y) * cam.zoom / LIGHT_SCALE;
  }

  float cx = (wx - cam.x) * cam.zoom / LIGHT_SCALE;
  float cy = (float)LIGHT_H - (wy - cam.y) * cam.zoom / LIGHT_SCALE;

  if (count > 0)
    drawLightPolygon(r, cx, cy, lm, count, inner);
}

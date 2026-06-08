#include "render/core/renderer.h"

void drawMushroom(Renderer &r, const SpriteFrame &frame, float x, float y,
                  bool flipX) {
  if (!r.mushroomLoaded) {
    drawRect(r, x + 1.f, y + 1.f, 13.f, 13.f, {180, 40, 40, 255});
    return;
  }
  drawFrom(r, r.mushroomTex, frame, x, y, flipX);
}

void drawSlime(Renderer &r, const SpriteFrame &frame, float x, float y,
               bool flipX) {
  if (!r.slimeLoaded) {
    drawRect(r, x + 1.f, y + 1.f, 13.f, 13.f, {60, 180, 60, 178});
    return;
  }
  drawFrom(r, r.slimeTex, frame, x, y, flipX, 178);
}

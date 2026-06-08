#include "render/core/renderer.h"

void drawTorch(Renderer &r, const SpriteFrame &frame, float x, float y) {
  if (!r.torchLoaded) {
    drawRect(r, x, y, 8.f, 24.f, {255, 160, 60, 200});
    return;
  }
  drawFrom(r, r.torchTex, frame, x, y);
}

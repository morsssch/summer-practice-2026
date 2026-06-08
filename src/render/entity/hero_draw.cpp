#include "render/core/renderer.h"

void drawSprite(Renderer &r, const SpriteFrame &frame, float x, float y,
                bool flipX) {
  drawFrom(r, r.heroTex, frame, x, y, flipX);
}

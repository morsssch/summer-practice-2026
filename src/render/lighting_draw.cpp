#include "renderer.h"
#include "game/lighting.h"
#include "core/types.h"

void addPointLight(Renderer& r, const Room& room, const Camera& cam,
                   float wx, float wy, float radius, sf::Color inner) {
    constexpr int MAXP = 256;
    float worldPts[MAXP * 2];
    float lm[MAXP * 2];
    int count = 0;
    buildLightPolygon(room, wx, wy, radius, cam, worldPts, count);

    for (int i = 0; i < count; i++) {
        lm[i * 2 + 0] = (worldPts[i * 2 + 0] - cam.x) * cam.zoom / LIGHT_SCALE;
        lm[i * 2 + 1] = (float)LIGHT_H - (worldPts[i * 2 + 1] - cam.y) * cam.zoom / LIGHT_SCALE;
    }

    float cx = (wx - cam.x) * cam.zoom / LIGHT_SCALE;
    float cy = (float)LIGHT_H - (wy - cam.y) * cam.zoom / LIGHT_SCALE;

    if (count > 0)
        drawLightPolygon(r, cx, cy, lm, count, inner);
}

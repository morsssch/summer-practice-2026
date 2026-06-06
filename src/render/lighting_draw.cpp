#include "renderer.h"
#include "game/lighting.h"
#include "core/types.h"
#include <vector>

void addPointLight(Renderer& r, const Room& room, const Camera& cam,
                   float wx, float wy, float radius, sf::Color inner) {
    const int MAXP = 256;
    float worldPts[MAXP * 2];
    int count = 0;
    buildLightPolygon(room, wx, wy, radius, cam, worldPts, count);

    std::vector<float> lm;
    lm.reserve(count * 2);
    for (int i = 0; i < count; i++) {
        float hx = worldPts[i * 2 + 0];
        float hy = worldPts[i * 2 + 1];
        float lx = (hx - cam.x) * cam.zoom / LIGHT_SCALE;
        float ly = (float)LIGHT_H - (hy - cam.y) * cam.zoom / LIGHT_SCALE;
        lm.push_back(lx);
        lm.push_back(ly);
    }

    float cx = (wx - cam.x) * cam.zoom / LIGHT_SCALE;
    float cy = (float)LIGHT_H - (wy - cam.y) * cam.zoom / LIGHT_SCALE;

    if (!lm.empty())
        drawLightPolygon(r, cx, cy, lm.data(), count, inner);
}

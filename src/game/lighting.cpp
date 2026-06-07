#include "lighting.h"
#include "game/tilemap.h"
#include "core/types.h"
#include <cmath>

float castRay(const Room& room, float ox, float oy, float dx, float dy, float maxDist) {
    const float STEP = 4.f;
    for (float t = STEP; t < maxDist; t += STEP) {
        float px = ox + dx * t, py = oy + dy * t;
        if (isSolid(room, px, py) || isPlatform(room, px, py))
            return t;
    }
    return maxDist;
}

void buildLightPolygon(const Room& room, float wx, float wy, float radius,
                       const Camera& /*cam*/, float* out, int& count) {
    const int RAYS = 90;
    count = 0;
    for (int i = 0; i <= RAYS; i++) {
        float a  = (float)i / RAYS * PI2;
        float dx = std::cos(a), dy = std::sin(a);
        float t  = castRay(room, wx, wy, dx, dy, radius);
        float hx = wx + dx * t;
        float hy = wy + dy * t;
        out[count * 2 + 0] = hx;
        out[count * 2 + 1] = hy;
        count++;
    }
}

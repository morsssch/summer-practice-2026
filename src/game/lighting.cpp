#include "lighting.h"
#include "game/tilemap.h"
#include "core/types.h"
#include <cmath>
#include <algorithm>

static float rayBox(float ox, float oy, float dx, float dy,
                    float bx, float by, float bw, float bh) {
    const float EPS = 1e-6f;
    if (std::abs(dx) < EPS && (ox < bx || ox > bx + bw)) return -1.f;
    if (std::abs(dy) < EPS && (oy < by || oy > by + bh)) return -1.f;

    float tx1 = (std::abs(dx) > EPS) ? (bx      - ox) / dx :  1e30f;
    float tx2 = (std::abs(dx) > EPS) ? (bx + bw - ox) / dx : -1e30f;
    float ty1 = (std::abs(dy) > EPS) ? (by      - oy) / dy :  1e30f;
    float ty2 = (std::abs(dy) > EPS) ? (by + bh - oy) / dy : -1e30f;

    float tmin = std::max(std::min(tx1, tx2), std::min(ty1, ty2));
    float tmax = std::min(std::max(tx1, tx2), std::max(ty1, ty2));

    if (tmax < 0.f || tmin > tmax || tmin <= 0.f) return -1.f;
    return tmin;
}

float castRay(const Room& room, float ox, float oy, float dx, float dy, float maxDist,
              const LightOccluder* occ, int occCount) {
    float best = maxDist;

    const float EXPAND = 4.f;
    for (int i = 0; i < occCount; i++) {
        if (ox >= occ[i].x - EXPAND && ox <= occ[i].x + occ[i].w + EXPAND &&
            oy >= occ[i].y - EXPAND && oy <= occ[i].y + occ[i].h + EXPAND) continue;
        float t = rayBox(ox, oy, dx, dy, occ[i].x, occ[i].y, occ[i].w, occ[i].h);
        if (t > 0.f && t < best) best = t;
    }

    const float STEP = 4.f;
    for (float t = STEP; t < best; t += STEP) {
        float px = ox + dx * t, py = oy + dy * t;
        if (isSolid(room, px, py) || isPlatform(room, px, py))
            return t;
    }
    return best;
}

void buildLightPolygon(const Room& room, float wx, float wy, float radius,
                       const Camera& /*cam*/, float* out, int& count,
                       const LightOccluder* occ, int occCount) {
    constexpr int MAX_ANGLES = 512;
    float angles[MAX_ANGLES];
    int nAngles = 0;

    const int RAYS = 90;
    for (int i = 0; i <= RAYS; i++)
        angles[nAngles++] = (float)i / RAYS * PI2;

    const float EPS_A = 0.0003f;
    const float EXPAND = 4.f;
    for (int i = 0; i < occCount && nAngles + 12 <= MAX_ANGLES; i++) {
        if (wx >= occ[i].x - EXPAND && wx <= occ[i].x + occ[i].w + EXPAND &&
            wy >= occ[i].y - EXPAND && wy <= occ[i].y + occ[i].h + EXPAND) continue;

        float cx[4] = { occ[i].x,             occ[i].x + occ[i].w,
                        occ[i].x,             occ[i].x + occ[i].w };
        float cy[4] = { occ[i].y,             occ[i].y,
                        occ[i].y + occ[i].h,  occ[i].y + occ[i].h };
        for (int c = 0; c < 4; c++) {
            float a = std::atan2(cy[c] - wy, cx[c] - wx);
            if (a < 0.f) a += PI2;
            angles[nAngles++] = a - EPS_A;
            angles[nAngles++] = a;
            angles[nAngles++] = a + EPS_A;
        }
    }

    std::sort(angles, angles + nAngles);

    count = 0;
    for (int i = 0; i < nAngles; i++) {
        float a  = angles[i];
        float dx = std::cos(a), dy = std::sin(a);
        float t  = castRay(room, wx, wy, dx, dy, radius, occ, occCount);
        out[count * 2 + 0] = wx + dx * t;
        out[count * 2 + 1] = wy + dy * t;
        count++;
    }
}

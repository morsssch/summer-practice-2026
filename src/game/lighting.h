#pragma once

#include "core/types.h"

float castRay(const Room& room, float ox, float oy, float dx, float dy, float maxDist);
void buildLightPolygon(const Room& room, float wx, float wy, float radius,
                       const Camera& cam, float* out, int& count);

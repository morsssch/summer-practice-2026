#pragma once

#include "core/types.h"
#include "render/core/renderer.h"
#include <SFML/Graphics.hpp>

void beginLightMap(Renderer &r, sf::Color ambient);
void drawLightQuad(Renderer &r, float x0, float y0, float x1, float y1,
                   float x2, float y2, float x3, float y3, sf::Color nearColor,
                   sf::Color farColor);
void drawLightPolygon(Renderer &r, float cx, float cy, const float *xys,
                      int count, sf::Color inner = sf::Color::White);
void applyLightMap(Renderer &r);

void addPointLight(Renderer &r, const Room &room, const Camera &cam, float wx,
                   float wy, float radius, sf::Color inner = sf::Color::White,
                   const LightOccluder *occ = nullptr, int occCount = 0);

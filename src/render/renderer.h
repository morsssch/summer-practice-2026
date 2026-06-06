#pragma once

#include <SFML/Graphics.hpp>
#include "sprites.h"
#include "core/types.h"

struct Renderer {
    sf::RenderWindow* window;
    sf::Texture       tilesetTex;
    sf::Texture       heroTex;
    sf::Texture       bgTex;
    sf::FloatRect     viewport;
};

bool initRenderer(Renderer& r, sf::RenderWindow& window);

void applyLetterbox(Renderer& r);
void setWorldView(Renderer& r, const Camera& cam);
void setUIView(Renderer& r);

void drawBackground(Renderer& r);
void drawTile(Renderer& r, const SpriteFrame& frame, int tileX, int tileY);
void drawSprite(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX = false);

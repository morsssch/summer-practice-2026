#pragma once

#include <SFML/Graphics.hpp>
#include "sprites.h"

struct Renderer {
    sf::RenderWindow* window;
    sf::Texture       tilesetTex;
    sf::Texture       heroTex;
    sf::Texture       bgTex;
};

bool initRenderer(Renderer& r, sf::RenderWindow& window);

void applyLetterbox(sf::RenderWindow& window);
void drawBackground(Renderer& r);
void drawTile(Renderer& r, const SpriteFrame& frame, int tileX, int tileY);
void drawSprite(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX = false);

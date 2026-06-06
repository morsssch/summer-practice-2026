#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "sprites.h"
#include "core/types.h"
#include "core/input.h"

struct Renderer {
    sf::RenderWindow* window;
    sf::Texture       tilesetTex;
    sf::Texture       heroTex;
    sf::Texture       bgTex;
    sf::Texture       grassTex;
    sf::FloatRect     viewport;
    sf::Font          font;
    bool              fontLoaded  = false;
    bool              grassLoaded = false;
    sf::Texture       logoTex;
    bool              logoLoaded  = false;
};

bool initRenderer(Renderer& r, sf::RenderWindow& window);

void applyLetterbox(Renderer& r);
void setWorldView(Renderer& r, const Camera& cam);
void setUIView(Renderer& r);

void drawBackground(Renderer& r);
void drawTile(Renderer& r, const SpriteFrame& frame, int tileX, int tileY);
void drawSprite(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX = false);
void drawRect(Renderer& r, float x, float y, float w, float h, sf::Color color);
void drawText(Renderer& r, const std::string& str, float cx, float cy, unsigned int size, sf::Color color);
void drawGrassSprite(Renderer& r, float x, float y, int tileCol, int tileRow, float angleDeg);

void updateMouseInput(Input& input, const Renderer& r);

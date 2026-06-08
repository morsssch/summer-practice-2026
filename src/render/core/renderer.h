#pragma once

#include "core/types.h"
#include "render/core/resources.h"
#include "render/core/sprites.h"
#include <SFML/Graphics.hpp>
#include <string>

struct Renderer : public RenderResources {
  sf::RenderWindow *window = nullptr;
  sf::FloatRect viewport;
};

struct Room;

bool initRenderer(Renderer &r, sf::RenderWindow &window);

void applyLetterbox(Renderer &r);
void setWorldView(Renderer &r, const Camera &cam);
void setUIView(Renderer &r);

void drawBackground(Renderer &r);
void drawTile(Renderer &r, const SpriteFrame &frame, int tileX, int tileY);
void drawFrom(Renderer &r, const sf::Texture &tex, const SpriteFrame &frame,
              float x, float y, bool flipX = false, uint8_t alpha = 255);
void drawSprite(Renderer &r, const SpriteFrame &frame, float x, float y,
                bool flipX = false);
void drawRect(Renderer &r, float x, float y, float w, float h, sf::Color color);
void drawText(Renderer &r, const std::string &str, float cx, float cy,
              unsigned int size, sf::Color color);
void drawTextLeft(Renderer &r, const std::string &str, float x, float cy,
                  unsigned int size, sf::Color color);
void drawGrassSprite(Renderer &r, float x, float y, int tileCol, int tileRow,
                     float angleDeg);
void drawMushroom(Renderer &r, const SpriteFrame &frame, float x, float y,
                  bool flipX = false);
void drawSlime(Renderer &r, const SpriteFrame &frame, float x, float y,
               bool flipX = false);
void drawTorch(Renderer &r, const SpriteFrame &frame, float x, float y);
void drawOrbDrop(Renderer &r, const SpriteFrame &frame, float x, float y,
                 uint8_t alpha = 255);
void drawOrbCollect(Renderer &r, const SpriteFrame &frame, float x, float y);
void drawPotion(Renderer &r, float x, float y, uint8_t alpha = 255);
void drawOrbHudIcon(Renderer &r, float x, float y);

constexpr int LIGHT_W = 240;
constexpr int LIGHT_H = 137;
constexpr float LIGHT_SCALE = 2.f;

void drawScanlines(Renderer &r);
void drawUISprite(Renderer &r, sf::Texture &tex, float x, float y);
void drawLostHeartFrame(Renderer &r, int frame, float x, float y);

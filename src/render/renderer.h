#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "sprites.h"
#include "core/types.h"
#include "core/input.h"

struct Renderer {
    sf::RenderWindow*  window;
    sf::Texture        tilesetTex;
    sf::Texture        heroTex;
    sf::Texture        bgTex;
    sf::Texture        grassTex;
    sf::FloatRect      viewport;
    sf::Font           font;
    bool               fontLoaded  = false;
    bool               grassLoaded = false;
    sf::Texture        logoTex;
    bool               logoLoaded     = false;
    sf::Texture        mushroomTex;
    bool               mushroomLoaded = false;
    sf::Texture        slimeTex;
    bool               slimeLoaded    = false;
    sf::Texture        torchTex;
    bool               torchLoaded    = false;
    sf::RenderTexture  lightMap;

    sf::Texture        hudHeartFullTex;
    sf::Texture        hudHeartEmptyTex;
    sf::Texture        hudBarLeftTex;
    sf::Texture        hudBarMiddleTex;
    sf::Texture        hudBarRightTex;
    sf::Texture        hudOrbTex;
    sf::Texture        hudHpPanelTex;
    sf::Texture        hudHeroIconTex;
    sf::Texture        hudLostHeartTex;
    bool               hudHeartsLoaded   = false;
    bool               hudBarLoaded      = false;
    bool               hudOrbLoaded      = false;
    bool               hudHpPanelLoaded  = false;
    bool               hudHeroIconLoaded = false;
    bool               hudLostHeartLoaded = false;
};

struct Room;

bool initRenderer(Renderer& r, sf::RenderWindow& window);

void applyLetterbox(Renderer& r);
void setWorldView(Renderer& r, const Camera& cam);
void setUIView(Renderer& r);

void drawBackground(Renderer& r);
void drawTile(Renderer& r, const SpriteFrame& frame, int tileX, int tileY);
void drawTilemap(Renderer& r, const Room& room);
void drawSprite(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX = false);
void drawRect(Renderer& r, float x, float y, float w, float h, sf::Color color);
void drawText(Renderer& r, const std::string& str, float cx, float cy, unsigned int size, sf::Color color);
void drawGrassSprite(Renderer& r, float x, float y, int tileCol, int tileRow, float angleDeg);
void drawMushroom(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX = false);
void drawSlime(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX = false);
void drawTorch(Renderer& r, const SpriteFrame& frame, float x, float y);

void updateMouseInput(Input& input, const Renderer& r);

constexpr int   LIGHT_W     = 240;
constexpr int   LIGHT_H     = 137;
constexpr float LIGHT_SCALE = 2.f;

void beginLightMap(Renderer& r, sf::Color color);
void drawLightQuad(Renderer& r, float x0, float y0, float x1, float y1,
                                 float x2, float y2, float x3, float y3,
                                 sf::Color nearColor, sf::Color farColor);
void drawLightPolygon(Renderer& r, float cx, float cy, const float* xys, int count, sf::Color inner = sf::Color::White);
void applyLightMap(Renderer& r);

void drawScanlines(Renderer& r);
void drawUISprite(Renderer& r, sf::Texture& tex, float x, float y);
void drawLostHeartFrame(Renderer& r, int frame, float x, float y);

void addPointLight(Renderer& r, const Room& room, const Camera& cam,
                   float wx, float wy, float radius, sf::Color inner = sf::Color::White,
                   const LightOccluder* occ = nullptr, int occCount = 0);

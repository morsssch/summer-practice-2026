#include "renderer.h"
#include "core/types.h"
#include <SFML/Graphics.hpp>
#include <cmath>

bool initRenderer(Renderer& r, sf::RenderWindow& window) {
    r.window = &window;
    if (!r.tilesetTex.loadFromFile("assets/tileset.png"))    return false;
    if (!r.heroTex.loadFromFile("assets/hero.png"))          return false;
    if (!r.bgTex.loadFromFile("assets/background.png"))      return false;
    r.fontLoaded  = r.font.openFromFile("assets/font.ttf");
    if (r.fontLoaded) r.font.setSmooth(false);
    r.grassLoaded = r.grassTex.loadFromFile("assets/grass.png");
    r.logoLoaded  = r.logoTex.loadFromFile("assets/logo.png");
    (void)r.lightMap.resize({(unsigned)LIGHT_W, (unsigned)LIGHT_H});
    r.lightMap.setSmooth(false);
    return true;
}

static sf::FloatRect calcLetterboxViewport(const sf::RenderWindow& window) {
    float targetRatio = (float)WINDOW_W / (float)WINDOW_H;
    float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
    if (windowRatio > targetRatio) {
        float w = targetRatio / windowRatio;
        return sf::FloatRect({ (1.f - w) / 2.f, 0.f }, { w, 1.f });
    } else {
        float h = windowRatio / targetRatio;
        return sf::FloatRect({ 0.f, (1.f - h) / 2.f }, { 1.f, h });
    }
}

void applyLetterbox(Renderer& r) {
    r.viewport = calcLetterboxViewport(*r.window);
}

void setWorldView(Renderer& r, const Camera& cam) {
    sf::View view(sf::FloatRect(
        { cam.x, cam.y },
        { WINDOW_W / cam.zoom, WINDOW_H / cam.zoom }
    ));
    view.setViewport(r.viewport);
    r.window->setView(view);
}

void setUIView(Renderer& r) {
    sf::View view(sf::FloatRect({0, 0}, {(float)WINDOW_W, (float)WINDOW_H}));
    view.setViewport(r.viewport);
    r.window->setView(view);
}

void drawBackground(Renderer& r) {
    sf::Sprite sprite(r.bgTex);
    sprite.setScale({ (float)WINDOW_W / 240.f, (float)WINDOW_H / 160.f });
    r.window->draw(sprite);
}

void drawTile(Renderer& r, const SpriteFrame& frame, int tileX, int tileY) {
    sf::Sprite sprite(r.tilesetTex);
    sprite.setTextureRect(sf::IntRect({ frame.x, frame.y }, { frame.w, frame.h }));
    sprite.setPosition({ (float)(tileX * 16), (float)(tileY * 16) });
    r.window->draw(sprite);
}

void updateMouseInput(Input& input, const Renderer& r) {
    static float prevX = -9999.f, prevY = -9999.f;

    auto winSize = r.window->getSize();
    float vpPxW  = r.viewport.size.x * winSize.x;
    float vpPxH  = r.viewport.size.y * winSize.y;

    if (vpPxW > 0.f && vpPxH > 0.f) {
        auto  raw   = sf::Mouse::getPosition(*r.window);
        float vpPxX = r.viewport.position.x * winSize.x;
        float vpPxY = r.viewport.position.y * winSize.y;
        input.mouseX = (raw.x - vpPxX) / vpPxW * WINDOW_W;
        input.mouseY = (raw.y - vpPxY) / vpPxH * WINDOW_H;
    }

    input.mouseMoved = (input.mouseX != prevX || input.mouseY != prevY);
    prevX = input.mouseX;
    prevY = input.mouseY;
}

void drawRect(Renderer& r, float x, float y, float w, float h, sf::Color color) {
    sf::RectangleShape rect({w, h});
    rect.setPosition({x, y});
    rect.setFillColor(color);
    r.window->draw(rect);
}

void drawText(Renderer& r, const std::string& str, float cx, float cy, unsigned int size, sf::Color color) {
    if (!r.fontLoaded) return;
    sf::Text text(r.font, sf::String::fromUtf8(str.begin(), str.end()), size);
    text.setFillColor(color);
    auto bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                     bounds.position.y + bounds.size.y / 2.f });
    text.setPosition({cx, cy});
    r.window->draw(text);
}

void drawGrassSprite(Renderer& r, float x, float y, int tileCol, int tileRow, float angleDeg) {
    if (!r.grassLoaded) return;
    sf::Sprite sprite(r.grassTex);
    sprite.setTextureRect(sf::IntRect({tileCol * 16, tileRow * 16}, {16, 16}));
    sprite.setOrigin({8.f, 16.f});
    sprite.setPosition({x, y});
    sprite.setRotation(sf::degrees(angleDeg));
    r.window->draw(sprite);
}

void beginLightMap(Renderer& r, sf::Color ambient) {
    r.lightMap.clear(ambient);
}

void drawLightQuad(Renderer& r,
    float x0, float y0, float x1, float y1,
    float x2, float y2, float x3, float y3,
    sf::Color nearColor, sf::Color farColor) {
    sf::VertexArray quad(sf::PrimitiveType::TriangleFan, 4);
    quad[0] = {{x0, y0}, nearColor};
    quad[1] = {{x1, y1}, nearColor};
    quad[2] = {{x2, y2}, farColor};
    quad[3] = {{x3, y3}, farColor};
    r.lightMap.draw(quad);
}

void drawLightPolygon(Renderer& r, float cx, float cy, const float* xys, int count, sf::Color inner) {
    sf::VertexArray fan(sf::PrimitiveType::TriangleFan, count + 1);
    fan[0].position = {cx, cy};
    fan[0].color    = inner;
    for (int i = 0; i < count; i++) {
        fan[i + 1].position = {xys[i * 2], xys[i * 2 + 1]};
        fan[i + 1].color    = sf::Color(inner.r, inner.g, inner.b, 0);
    }
    r.lightMap.draw(fan);
}

void applyLightMap(Renderer& r) {
    r.lightMap.display();
    sf::Sprite overlay(r.lightMap.getTexture());
    overlay.setScale({LIGHT_SCALE, -LIGHT_SCALE});
    overlay.setPosition({0.f, (float)WINDOW_H});
    r.window->draw(overlay, sf::BlendMultiply);
}

void drawScanlines(Renderer& r) {
    const int step = 2;
    const uint8_t alpha = 15;
    sf::VertexArray lines(sf::PrimitiveType::Triangles, (WINDOW_H / step) * 6);
    int idx = 0;
    for (int y = 0; y < WINDOW_H; y += step) {
        sf::Color c(52, 10, 44, alpha);
        lines[idx++] = {{0.f,          (float)y},        c};
        lines[idx++] = {{(float)WINDOW_W, (float)y},        c};
        lines[idx++] = {{(float)WINDOW_W, (float)(y + 1)},  c};
        lines[idx++] = {{0.f,          (float)y},        c};
        lines[idx++] = {{(float)WINDOW_W, (float)(y + 1)},  c};
        lines[idx++] = {{0.f,          (float)(y + 1)},  c};
    }
    r.window->draw(lines);
}

void drawSprite(Renderer& r, const SpriteFrame& frame, float x, float y, bool flipX) {
    sf::Sprite sprite(r.heroTex);
    sprite.setTextureRect(sf::IntRect({ frame.x, frame.y }, { frame.w, frame.h }));
    if (flipX) {
        sprite.setScale({ -1.f, 1.f });
        sprite.setPosition({ x + frame.w, y });
    } else {
        sprite.setPosition({ x, y });
    }
    r.window->draw(sprite);
}

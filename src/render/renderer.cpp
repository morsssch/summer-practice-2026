#include "renderer.h"
#include "core/types.h"
#include <SFML/Graphics.hpp>

bool initRenderer(Renderer& r, sf::RenderWindow& window) {
    r.window = &window;
    if (!r.tilesetTex.loadFromFile("assets/tileset.png"))    return false;
    if (!r.heroTex.loadFromFile("assets/hero.png"))          return false;
    if (!r.bgTex.loadFromFile("assets/background.png"))      return false;
    r.fontLoaded  = r.font.openFromFile("assets/font.ttf");
    if (r.fontLoaded) r.font.setSmooth(false);
    r.grassLoaded = r.grassTex.loadFromFile("assets/grass.png");
    r.logoLoaded  = r.logoTex.loadFromFile("assets/logo.png");
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

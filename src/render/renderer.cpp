#include "renderer.h"
#include "core/types.h"
#include <SFML/Graphics.hpp>

bool initRenderer(Renderer& r, sf::RenderWindow& window) {
    r.window = &window;
    if (!r.tilesetTex.loadFromFile("assets/tileset.png"))    return false;
    if (!r.heroTex.loadFromFile("assets/hero.png"))          return false;
    if (!r.bgTex.loadFromFile("assets/background.png"))      return false;
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

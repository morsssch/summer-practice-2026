#include "renderer.h"
#include <SFML/Graphics.hpp>

bool initRenderer(Renderer& r, sf::RenderWindow& window) {
    r.window = &window;
    if (!r.tilesetTex.loadFromFile("assets/tileset.png"))    return false;
    if (!r.heroTex.loadFromFile("assets/hero.png"))          return false;
    if (!r.bgTex.loadFromFile("assets/background.png"))      return false;
    return true;
}

void applyLetterbox(sf::RenderWindow& window) {
    float targetRatio = 640.f / 352.f;
    float windowRatio = (float)window.getSize().x / (float)window.getSize().y;

    sf::FloatRect viewport;
    if (windowRatio > targetRatio) {
        float w = targetRatio / windowRatio;
        viewport = sf::FloatRect({ (1.f - w) / 2.f, 0.f }, { w, 1.f });
    } else {
        float h = windowRatio / targetRatio;
        viewport = sf::FloatRect({ 0.f, (1.f - h) / 2.f }, { 1.f, h });
    }

    sf::View view(sf::FloatRect({0, 0}, {640, 352}));
    view.setViewport(viewport);
    window.setView(view);
}

void drawBackground(Renderer& r) {
    sf::Sprite sprite(r.bgTex);
    sprite.setScale({ 640.f / 240.f, 352.f / 160.f });
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

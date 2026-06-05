#include "renderer.h"
#include <SFML/Graphics.hpp>

bool initRenderer(Renderer& r, sf::RenderWindow& window) {
    r.window = &window;
    if (!r.tilesetTex.loadFromFile("assets/tileset.png")) return false;
    if (!r.heroTex.loadFromFile("assets/hero.png"))       return false;
    return true;
}

void drawTile(Renderer& r, const SpriteFrame& frame, int tileX, int tileY) {
    sf::Sprite sprite(r.tilesetTex);
    sprite.setTextureRect(sf::IntRect({ frame.x, frame.y }, { frame.w, frame.h }));
    sprite.setPosition({ (float)(tileX * 16), (float)(tileY * 16) });
    r.window->draw(sprite);
}

void drawSprite(Renderer& r, const SpriteFrame& frame, float x, float y) {
    sf::Sprite sprite(r.heroTex);
    sprite.setTextureRect(sf::IntRect({ frame.x, frame.y }, { frame.w, frame.h }));
    sprite.setPosition({ x, y });
    r.window->draw(sprite);
}

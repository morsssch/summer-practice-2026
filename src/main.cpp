#include <SFML/Graphics.hpp>
#include "render/renderer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 704}), "Platformer");
    window.setView(sf::View(sf::FloatRect({0, 0}, {640, 352})));

    Renderer renderer;
    if (!initRenderer(renderer, window)) return -1;

    sf::Clock clock;
    int fpsCounter = 0;
    float fpsTimer = 0.f;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        fpsTimer += dt;
        fpsCounter++;

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (fpsTimer >= 1.0f)
        {
            window.setTitle("Platformer | FPS: " + std::to_string(fpsCounter));
            fpsTimer = 0.f;
            fpsCounter = 0;
        }

        window.clear();

        for (int x = 0; x < 40; x++) {
            for (int y = 0; y < 22; y++) {
                if (y == 20) drawTile(renderer, TILE_GROUND, x, y);
                else if (y == 19) drawTile(renderer, TILE_DARK_TOP, x, y);
                else if (y == 18) drawTile(renderer, TILE_DARK, x, y);
                else if (y <= 21) drawTile(renderer, TILE_GRASS_TOP, x, y);
                
            }
        }

        window.display();
        window.setFramerateLimit(240);
    }
}
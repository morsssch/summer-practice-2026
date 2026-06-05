#include <SFML/Graphics.hpp>
#include "render/renderer.h"
#include "game/tilemap.h"
#include "game/player.h"
#include "core/input.h"
#include "levels/screen_01.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 704}), "Platformer");
    applyLetterbox(window);

    Renderer renderer;
    if (!initRenderer(renderer, window))
        return -1;

    Player player;
    player.pos = { 32.f, 280.f };

    Input input;

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
            if (event->is<sf::Event::Resized>())
                applyLetterbox(window);
        }

        updateInput(input, window);
        updatePlayer(player, input, dt);

        if (fpsTimer >= 1.0f)
        {
            window.setTitle("Platformer | FPS: " + std::to_string(fpsCounter));
            fpsTimer = 0.f;
            fpsCounter = 0;
        }

        window.clear();
        drawBackground(renderer);
        drawTilemap(renderer, SCREEN_01);
        drawPlayer(renderer, player);

        window.display();
        window.setFramerateLimit(60);
    }
}

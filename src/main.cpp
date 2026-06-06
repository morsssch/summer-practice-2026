#include <SFML/Graphics.hpp>
#include "render/renderer.h"
#include "game/tilemap.h"
#include "game/player.h"
#include "core/input.h"
#include "levels/screen_01.h"
#include "screens/game_screen.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_W*2, WINDOW_H*2}), "Platformer");

    Renderer renderer;
    if (!initRenderer(renderer, window))
        return -1;

    applyLetterbox(renderer);

    Player player;
    player.pos = { 80.f, 498.f };

    Camera camera = { 0.f, 272.f };

    Input input;

    sf::Clock clock;
    int fpsCounter = 0;
    float fpsTimer = 0.f;

    while (window.isOpen())
    {
        float dt = std::min(clock.restart().asSeconds(), 0.05f);
        fpsTimer += dt;
        fpsCounter++;

        applyLetterbox(renderer);

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        updateInput(input, window);
        updatePlayer(player, input, SCREEN_01, dt);
        updateCamera(camera, player, SCREEN_01, dt);

        if (fpsTimer >= 1.0f)
        {
            window.setTitle("Platformer | FPS: " + std::to_string(fpsCounter));
            fpsTimer = 0.f;
            fpsCounter = 0;
        }

        window.clear();
        setUIView(renderer);
        drawBackground(renderer);
        setWorldView(renderer, camera);
        drawTilemap(renderer, SCREEN_01);
        drawPlayer(renderer, player);

        window.display();
        window.setFramerateLimit(240);
    }
}

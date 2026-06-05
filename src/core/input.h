#pragma once

#include <SFML/Window.hpp>

struct Input {
    bool left        = false;
    bool right       = false;
    bool jump        = false;
    bool jumpPressed = false;
    bool roll        = false;
};

void updateInput(Input& input, const sf::Window& window);

#pragma once

#include <SFML/Graphics.hpp>

struct Input {
  bool left = false;
  bool right = false;
  bool jump = false;
  bool jumpPressed = false;
  bool rollPressed = false;
  bool attackPressed = false;

  bool leftPressed = false;
  bool rightPressed = false;
  bool upPressed = false;
  bool downPressed = false;
  bool confirmPressed = false;

  float mouseX = 0.f;
  float mouseY = 0.f;
  bool mouseClicked = false;
  bool mouseMoved = false;
  bool escapePressed = false;
};

void updateInput(Input &input, const sf::Window &window);
void updateMouseInput(Input &input, const sf::Window &window,
                      const sf::FloatRect &viewport);

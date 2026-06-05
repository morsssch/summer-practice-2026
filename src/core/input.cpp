#include "input.h"

void updateInput(Input& input, const sf::Window& window) {
    bool prevJump   = input.jump;
    bool prevRoll   = input.roll;
    bool prevAttack = input.attackPressed;

    input.left  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    input.jump  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    input.roll  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);
    bool attackKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);

    input.jumpPressed   = input.jump && !prevJump;
    input.roll          = input.roll && !prevRoll;
    input.attackPressed = attackKey && !prevAttack;
}

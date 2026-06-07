#include "input.h"

void updateInput(Input& input, const sf::Window& window) {
    static bool prevLeft    = false;
    static bool prevRight   = false;
    static bool prevUp      = false;
    static bool prevDown    = false;
    static bool prevConfirm = false;
    static bool prevMouse   = false;
    static bool prevEscape  = false;
    static bool prevRoll    = false;
    static bool prevAttack  = false;

    bool prevJump   = input.jump;

    input.left  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    input.jump  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    bool rawRoll    = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::X);
    bool attackKey  = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Z);
    bool upKey      = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    bool downKey    = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
    bool confirmKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
    bool mouseBtn   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool escapeKey  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);

    input.jumpPressed    = input.jump  && !prevJump;
    input.rollPressed    = rawRoll     && !prevRoll;
    input.attackPressed  = attackKey   && !prevAttack;
    input.leftPressed    = input.left  && !prevLeft;
    input.rightPressed   = input.right && !prevRight;
    input.upPressed      = upKey       && !prevUp;
    input.downPressed    = downKey     && !prevDown;
    input.confirmPressed = confirmKey  && !prevConfirm;
    input.mouseClicked   = mouseBtn    && !prevMouse;
    input.escapePressed  = escapeKey   && !prevEscape;

    prevLeft    = input.left;
    prevRight   = input.right;
    prevUp      = upKey;
    prevDown    = downKey;
    prevConfirm = confirmKey;
    prevMouse   = mouseBtn;
    prevEscape  = escapeKey;
    prevRoll    = rawRoll;
    prevAttack  = attackKey;
}

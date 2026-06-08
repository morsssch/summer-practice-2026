#include "screens/game/game_screen.h"

AppState updateGameImpl(GameState &s, const Input &input, float dt);
void drawGameImpl(Renderer &r, const GameState &s);

AppState updateGame(GameState &s, const Input &input, float dt) {
  return updateGameImpl(s, input, dt);
}

void drawGame(Renderer &r, const GameState &s) { drawGameImpl(r, s); }

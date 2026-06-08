#include "game/state/game_state.h"
#include "render/core/renderer.h"
#include "render/effects/lighting.h"
#include "screens/common/hud.h"
#include "screens/game/game_draw.h"
#include <cmath>

static Camera makeShakyCamera(const Camera &cam, float shakeTimer) {
  Camera shakyCam = cam;
  if (shakeTimer > 0.f) {
    float m = 3.f * (shakeTimer / 0.55f);
    shakyCam.x += (float)(rand() % 5 - 2) * m;
    shakyCam.y += (float)(rand() % 5 - 2) * m;
  }
  return shakyCam;
}

static void drawGameWorld(Renderer &r, const GameState &s) {
  setWorldView(r, makeShakyCamera(s.camera, s.camShakeTimer));
  drawStaticWorld(r, s);
  drawEntities(r, s);
}

static void drawGameLighting(Renderer &r, const GameState &s) {
  beginLightMap(r, sf::Color{255, 240, 200, 255});
  drawLightOccluders(r, s);
  setUIView(r);
  applyLightMap(r);
}

static void drawGameUI(Renderer &r, const GameState &s) {
  drawScanlines(r);
  drawHUD(r, s.player, s.artifacts, ARTIFACT_COUNT);
}

static void drawGameOverlay(Renderer &r, const GameState &s) {
  if (s.deathScreen)
    drawDeathOverlay(r, s);
}

void drawGameImpl(Renderer &r, const GameState &s) {
  setUIView(r);
  drawBackground(r);
  drawGameWorld(r, s);
  drawGameLighting(r, s);
  drawGameUI(r, s);
  drawGameOverlay(r, s);
}

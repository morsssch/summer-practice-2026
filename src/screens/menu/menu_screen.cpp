#include "menu_screen.h"
#include "core/types.h"
#include "render/core/renderer.h"

static const char *ITEM_LABELS[NUM_MENU_ITEMS] = {"Играть", "Настройки",
                                                  "Выход"};

AppState updateMenu(MenuState &s, const Input &input) {
  if (input.upPressed || input.downPressed)
    s.keyboardMode = true;
  if (input.mouseMoved)
    s.keyboardMode = false;

  if (input.upPressed)
    s.selectedItem = (s.selectedItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
  if (input.downPressed)
    s.selectedItem = (s.selectedItem + 1) % NUM_MENU_ITEMS;

  s.isHovering = false;
  bool mouseSelect = false;
  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    float x0 = MENU_ITEM_X - MENU_ITEM_W / 2.f;
    float y0 = MENU_ITEM_Y0 + i * MENU_ITEM_DY - MENU_ITEM_H / 2.f;
    if (input.mouseX >= x0 && input.mouseX <= x0 + MENU_ITEM_W &&
        input.mouseY >= y0 && input.mouseY <= y0 + MENU_ITEM_H) {
      s.isHovering = true;
      if (!s.keyboardMode)
        s.selectedItem = i;
      if (input.mouseClicked) {
        s.selectedItem = i;
        mouseSelect = true;
      }
    }
  }

  if (input.confirmPressed || mouseSelect) {
    if (s.selectedItem == 0)
      return AppState::GAME;
    if (s.selectedItem == 1)
      return AppState::SETTINGS;
    if (s.selectedItem == 2)
      return AppState::EXIT;
  }

  return AppState::MENU;
}

void drawMenu(Renderer &r, const MenuState &s) {
  setUIView(r);
  drawBackground(r);
  drawRect(r, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, {8, 8, 24, 180});

  drawText(r, "ПЛАТФОРМЕР", WINDOW_W / 2.f, 68.f, 16, {180, 220, 255, 255});

  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    float cy = MENU_ITEM_Y0 + i * MENU_ITEM_DY;
    bool sel = (i == s.selectedItem);

    if (sel)
      drawRect(r, MENU_ITEM_X - MENU_ITEM_W / 2.f, cy - MENU_ITEM_H / 2.f,
               MENU_ITEM_W, MENU_ITEM_H, {255, 255, 255, 25});

    sf::Color col =
        sel ? sf::Color{255, 220, 80, 255} : sf::Color{160, 160, 180, 255};
    drawText(r, ITEM_LABELS[i], MENU_ITEM_X, cy, 16, col);
  }
}

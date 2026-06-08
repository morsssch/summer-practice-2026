#include "pause_screen.h"
#include "core/types.h"
#include "render/core/renderer.h"

static const char *PAUSE_LABELS[NUM_PAUSE_ITEMS] = {
    "Продолжить", "Главное меню", "Настройки", "Выйти из игры"};

static bool hitTest(float cx, float cy, float hw, float hh, float mx,
                    float my) {
  return mx >= cx - hw && mx <= cx + hw && my >= cy - hh && my <= cy + hh;
}

AppState updatePause(PauseState &s, const Input &input) {
  if (s.showConfirm) {
    bool hoverNo = hitTest(CONFIRM_NO_CX, CONFIRM_BTN_Y, CONFIRM_BTN_HW,
                           CONFIRM_BTN_HH, input.mouseX, input.mouseY);
    bool hoverYes = hitTest(CONFIRM_YES_CX, CONFIRM_BTN_Y, CONFIRM_BTN_HW,
                            CONFIRM_BTN_HH, input.mouseX, input.mouseY);

    s.isHovering = hoverNo || hoverYes;
    if (input.mouseMoved) {
      if (hoverNo)
        s.confirmSel = 0;
      if (hoverYes)
        s.confirmSel = 1;
    }
    if (input.mouseClicked) {
      if (hoverNo) {
        s.showConfirm = false;
        return AppState::PAUSE;
      }
      if (hoverYes)
        return AppState::EXIT;
    }

    if (input.leftPressed || input.rightPressed)
      s.confirmSel = 1 - s.confirmSel;
    if (input.escapePressed) {
      s.showConfirm = false;
    } else if (input.confirmPressed) {
      if (s.confirmSel == 1)
        return AppState::EXIT;
      s.showConfirm = false;
    }
    return AppState::PAUSE;
  }

  if (input.upPressed || input.downPressed)
    s.keyboardMode = true;
  if (input.mouseMoved)
    s.keyboardMode = false;

  if (input.upPressed)
    s.selectedItem = (s.selectedItem - 1 + NUM_PAUSE_ITEMS) % NUM_PAUSE_ITEMS;
  if (input.downPressed)
    s.selectedItem = (s.selectedItem + 1) % NUM_PAUSE_ITEMS;
  if (input.escapePressed)
    return AppState::GAME;

  s.isHovering = false;
  bool mouseSelect = false;
  for (int i = 0; i < NUM_PAUSE_ITEMS; i++) {
    float x0 = PAUSE_ITEM_X - PAUSE_ITEM_W / 2.f;
    float y0 = PAUSE_ITEM_Y0 + i * PAUSE_ITEM_DY - PAUSE_ITEM_H / 2.f;
    if (input.mouseX >= x0 && input.mouseX <= x0 + PAUSE_ITEM_W &&
        input.mouseY >= y0 && input.mouseY <= y0 + PAUSE_ITEM_H) {
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
    switch (s.selectedItem) {
    case 0:
      return AppState::GAME;
    case 1:
      return AppState::MENU;
    case 2:
      return AppState::SETTINGS;
    case 3:
      s.showConfirm = true;
      s.confirmSel = 0;
      break;
    }
  }

  return AppState::PAUSE;
}

void drawPause(Renderer &r, const PauseState &s) {
  setUIView(r);
  drawRect(r, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, {0, 0, 0, 160});

  drawText(r, "ПАУЗА", WINDOW_W / 2.f, 62.f, 16, {180, 220, 255, 255});

  for (int i = 0; i < NUM_PAUSE_ITEMS; i++) {
    float cy = PAUSE_ITEM_Y0 + i * PAUSE_ITEM_DY;
    bool sel = (i == s.selectedItem);

    if (sel)
      drawRect(r, PAUSE_ITEM_X - PAUSE_ITEM_W / 2.f, cy - PAUSE_ITEM_H / 2.f,
               PAUSE_ITEM_W, PAUSE_ITEM_H, {255, 255, 255, 25});

    sf::Color col =
        sel ? sf::Color{255, 220, 80, 255} : sf::Color{160, 160, 180, 255};
    drawText(r, PAUSE_LABELS[i], PAUSE_ITEM_X, cy, 16, col);
  }

  if (s.showConfirm) {
    drawRect(r, CONFIRM_BX, CONFIRM_BY, 220.f, 60.f, {20, 20, 40, 235});
    drawRect(r, CONFIRM_BX, CONFIRM_BY, 220.f, 60.f, {100, 100, 160, 100});

    drawText(r, "Выйти из игры?", WINDOW_W / 2.f, CONFIRM_BY + 16.f, 16,
             {220, 220, 220, 255});

    sf::Color noCol = (s.confirmSel == 0) ? sf::Color{255, 220, 80, 255}
                                          : sf::Color{140, 140, 160, 255};
    sf::Color yesCol = (s.confirmSel == 1) ? sf::Color{255, 100, 80, 255}
                                           : sf::Color{140, 140, 160, 255};
    drawText(r, "Нет", CONFIRM_NO_CX, CONFIRM_BTN_Y, 16, noCol);
    drawText(r, "Да", CONFIRM_YES_CX, CONFIRM_BTN_Y, 16, yesCol);
  }
}

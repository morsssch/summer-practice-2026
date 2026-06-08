#pragma once
#include "core/app_state.h"
#include "core/input.h"

constexpr int NUM_PAUSE_ITEMS = 4;
constexpr float PAUSE_ITEM_X = 240.f;
constexpr float PAUSE_ITEM_Y0 = 97.f;
constexpr float PAUSE_ITEM_DY = 26.f;
constexpr float PAUSE_ITEM_W = 160.f;
constexpr float PAUSE_ITEM_H = 22.f;

struct Renderer;

constexpr float CONFIRM_BX = 130.f;
constexpr float CONFIRM_BY = 106.f;
constexpr float CONFIRM_NO_CX = 204.f;
constexpr float CONFIRM_YES_CX = 276.f;
constexpr float CONFIRM_BTN_Y = 150.f;
constexpr float CONFIRM_BTN_HW = 36.f;
constexpr float CONFIRM_BTN_HH = 12.f;

struct PauseState {
  int selectedItem = 0;
  bool isHovering = false;
  bool keyboardMode = false;
  bool showConfirm = false;
  int confirmSel = 0;
};

AppState updatePause(PauseState &s, const Input &input);
void drawPause(Renderer &r, const PauseState &s);

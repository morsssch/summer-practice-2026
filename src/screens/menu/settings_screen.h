#pragma once
#include "core/app_state.h"
#include "core/input.h"
#include "core/settings.h"

constexpr float SETTINGS_ITEM_X = 240.f;
constexpr float SETTINGS_ITEM_Y0 = 130.f;
constexpr float SETTINGS_ITEM_DY = 32.f;
constexpr float SETTINGS_ITEM_W = 200.f;
constexpr float SETTINGS_ITEM_H = 22.f;

struct Renderer;

struct SettingsState {
  int selectedItem = 0;
  AppState returnTo = AppState::MENU;
  bool isHovering = false;
  bool keyboardMode = false;
};

AppState updateSettings(SettingsState &s, const Input &input,
                        SettingsData &data);
void drawSettings(Renderer &r, const SettingsState &s,
                  const SettingsData &data);

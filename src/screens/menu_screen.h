#pragma once
#include "core/app_state.h"
#include "core/input.h"

constexpr int   NUM_MENU_ITEMS = 3;
constexpr float MENU_ITEM_X   = 240.f;
constexpr float MENU_ITEM_Y0  = 115.f;
constexpr float MENU_ITEM_DY  = 28.f;
constexpr float MENU_ITEM_W   = 140.f;
constexpr float MENU_ITEM_H   = 22.f;

struct Renderer;

struct MenuState {
    int  selectedItem = 0;
    bool isHovering   = false;
    bool keyboardMode = false;
};

AppState updateMenu(MenuState& s, const Input& input);
void     drawMenu(Renderer& r, const MenuState& s);

#include "settings_screen.h"
#include "render/renderer.h"
#include "core/types.h"
#include <algorithm>
#include <string>

AppState updateSettings(SettingsState& s, const Input& input, SettingsData& data) {
    if (s.selectedItem == 0) {
        if (input.leftPressed)  data.volume = std::max(0,   data.volume - 10);
        if (input.rightPressed) data.volume = std::min(100, data.volume + 10);
    }

    if (input.upPressed || input.downPressed) s.keyboardMode = true;
    if (input.mouseMoved)                    s.keyboardMode = false;

    if (input.upPressed)
        s.selectedItem = (s.selectedItem - 1 + 2) % 2;
    if (input.downPressed)
        s.selectedItem = (s.selectedItem + 1) % 2;
    if (input.escapePressed)
        return s.returnTo;

    s.isHovering = false;
    float x0 = SETTINGS_ITEM_X - SETTINGS_ITEM_W / 2.f;
    float y0 = SETTINGS_ITEM_Y0 + SETTINGS_ITEM_DY - SETTINGS_ITEM_H / 2.f;
    if (input.mouseX >= x0 && input.mouseX <= x0 + SETTINGS_ITEM_W &&
        input.mouseY >= y0 && input.mouseY <= y0 + SETTINGS_ITEM_H) {
        s.isHovering   = true;
        s.selectedItem = 1;
        if (input.mouseClicked) return s.returnTo;
    }

    if (input.confirmPressed && s.selectedItem == 1)
        return s.returnTo;

    return AppState::SETTINGS;
}

void drawSettings(Renderer& r, const SettingsState& s, const SettingsData& data) {
    setUIView(r);
    drawBackground(r);
    drawRect(r, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, {8, 8, 24, 200});

    drawText(r, "НАСТРОЙКИ", WINDOW_W / 2.f, 68.f, 16, {180, 220, 255, 255});

    std::string volStr = "< Громкость: " + std::to_string(data.volume) + "% >";
    bool volSel = (s.selectedItem == 0);
    if (volSel)
        drawRect(r,
            SETTINGS_ITEM_X - SETTINGS_ITEM_W / 2.f,
            SETTINGS_ITEM_Y0 - SETTINGS_ITEM_H / 2.f,
            SETTINGS_ITEM_W, SETTINGS_ITEM_H, {255, 255, 255, 25});
    sf::Color volCol = volSel ? sf::Color{255, 220, 80, 255} : sf::Color{160, 160, 180, 255};
    drawText(r, volStr, SETTINGS_ITEM_X, SETTINGS_ITEM_Y0, 16, volCol);

    float backY  = SETTINGS_ITEM_Y0 + SETTINGS_ITEM_DY;
    bool backSel = (s.selectedItem == 1);
    if (backSel)
        drawRect(r,
            SETTINGS_ITEM_X - SETTINGS_ITEM_W / 2.f,
            backY - SETTINGS_ITEM_H / 2.f,
            SETTINGS_ITEM_W, SETTINGS_ITEM_H, {255, 255, 255, 25});
    sf::Color backCol = backSel ? sf::Color{255, 220, 80, 255} : sf::Color{160, 160, 180, 255};
    drawText(r, "Назад", SETTINGS_ITEM_X, backY, 16, backCol);
}

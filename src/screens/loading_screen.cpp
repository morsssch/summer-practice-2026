#include "loading_screen.h"
#include "render/renderer.h"
#include "core/types.h"

AppState updateLoading(LoadingState& s, float dt) {
    s.timer += dt;
    if (s.timer >= 1.5f)
        return AppState::MENU;
    return AppState::LOADING;
}

void drawLoading(Renderer& r, const LoadingState& s) {
    setUIView(r);
    drawRect(r, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, {0, 0, 0, 255});

    // TODO: вставить лого
    // if (r.logoLoaded) {
    //     auto sz = r.logoTex.getSize();
    //     sf::Sprite logo(r.logoTex);
    //     logo.setPosition({ (WINDOW_W - (float)sz.x) / 2.f, 70.f });
    //     r.window->draw(logo);
    // }

    float progress = s.timer / 1.5f;
    float barW = WINDOW_W * 0.5f;
    float barX = (WINDOW_W - barW) / 2.f;
    float barY = WINDOW_H - 36.f;

    drawRect(r, barX, barY, barW, 4.f, {40, 40, 40, 255});
    drawRect(r, barX, barY, barW * progress, 4.f, {200, 200, 200, 255});

    drawText(r, "Загрузка...", WINDOW_W / 2.f, WINDOW_H - 18.f, 16, {100, 100, 100, 255});
}
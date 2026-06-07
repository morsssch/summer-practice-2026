#include "hud.h"
#include "game_screen.h"

static const sf::Color ART_COLORS[5] = {
    {240, 200,  40, 255},
    { 40, 200, 240, 255},
    {180,  60, 240, 255},
    {240, 120,  40, 255},
    { 60, 220,  80, 255},
};

static constexpr float HP_PANEL_X  = 4.f;
static constexpr float HP_PANEL_Y  = 4.f;
static constexpr float HP_ICON_X   = HP_PANEL_X + 4.f;
static constexpr float HP_ICON_Y   = HP_PANEL_Y + 8.f;
static constexpr float HP_HEARTS_X = HP_PANEL_X + 24.f;
static constexpr float HP_HEARTS_Y = HP_PANEL_Y + 8.f;

void drawHUD(Renderer& r, const Player& p, const bool* artifacts, int artifactCount) {
    setUIView(r);

    if (r.hudHeroIconLoaded)
        drawUISprite(r, r.hudHeroIconTex, HP_ICON_X, HP_ICON_Y);

    bool blink = p.iframeTimer > 0.f && (int)(p.iframeTimer * 10.f) % 2 == 0;
    for (int i = 0; i < p.maxHp; i++) {
        float hx = HP_HEARTS_X + i * 16.f;
        bool  full = i < p.currentHp;

        if (!full && p.lostHeartTimer >= 0.f && p.lostHeartSlot == i) {
            drawLostHeartFrame(r, p.lostHeartFrame, hx, HP_HEARTS_Y);
            continue;
        }

        if (r.hudHeartsLoaded) {
            if (full) {
                if (!blink) drawUISprite(r, r.hudHeartFullTex, hx, HP_HEARTS_Y);
            } else {
                drawUISprite(r, r.hudHeartEmptyTex, hx, HP_HEARTS_Y);
            }
        } else {
            sf::Color fill = full ? (blink ? sf::Color{220,50,50,120} : sf::Color{220,50,50,255})
                                  : sf::Color{60,20,20,255};
            drawRect(r, hx+1.f, HP_HEARTS_Y+1.f, 13.f, 13.f, fill);
        }
    }

    const float ART_X0 = 4.f;
    const float ART_Y  = (float)WINDOW_H - 15.f;
    const float ART_SZ = 9.f;
    const float ART_GAP = 2.f;

    for (int i = 0; i < artifactCount; i++) {
        float x = ART_X0 + i * (ART_SZ + ART_GAP);
        if (artifacts[i]) {
            drawRect(r, x, ART_Y, ART_SZ, ART_SZ, {20, 20, 20, 200});
            drawRect(r, x+1.f, ART_Y+1.f, ART_SZ-2.f, ART_SZ-2.f, ART_COLORS[i]);
        } else {
            drawRect(r, x, ART_Y, ART_SZ, ART_SZ, {20, 20, 20, 140});
            drawRect(r, x+1.f, ART_Y+1.f, ART_SZ-2.f, ART_SZ-2.f, {45, 45, 45, 140});
        }
    }
}

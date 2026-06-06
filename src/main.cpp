#include <SFML/Graphics.hpp>
#include <algorithm>
#include <optional>
#include "render/renderer.h"
#include "core/input.h"
#include "core/app_state.h"
#include "core/settings.h"
#include "screens/transition.h"
#include "screens/loading_screen.h"
#include "screens/menu_screen.h"
#include "screens/settings_screen.h"
#include "screens/game_screen.h"
#include "screens/pause_screen.h"

static void initNewState(AppState to, AppState from,
                         GameState& game, MenuState& menu,
                         PauseState& pause, SettingsState& settings,
                         sf::RenderWindow& window)
{
    if (to == AppState::EXIT)                        { window.close(); return; }
    if (to == AppState::GAME && from != AppState::PAUSE) initGameState(game);
    if (to == AppState::MENU)     menu     = MenuState{};
    if (to == AppState::PAUSE)    pause    = PauseState{};
    if (to == AppState::SETTINGS) { settings = SettingsState{}; settings.returnTo = from; }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_W*2, WINDOW_H*2}), "Platformer");

    Renderer renderer;
    if (!initRenderer(renderer, window)) return -1;
    applyLetterbox(renderer);

    auto handOpt   = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);
    auto arrowOpt  = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
    bool hasCursors   = handOpt.has_value() && arrowOpt.has_value();
    bool prevHovering = false;

    Input        input;
    AppState     state = AppState::LOADING;
    Transition   transition;
    SettingsData settingsData;

    LoadingState  loading;
    MenuState     menu;
    SettingsState settings;
    GameState     game;
    PauseState    pause;

    sf::Clock clock;
    int   fpsCounter = 0;
    float fpsTimer   = 0.f;

    while (window.isOpen())
    {
        float dt = std::min(clock.restart().asSeconds(), 0.05f);
        fpsTimer += dt;
        fpsCounter++;

        applyLetterbox(renderer);

        while (auto event = window.pollEvent())
            if (event->is<sf::Event::Closed>()) window.close();

        if (!transition.active)
        {
            updateInput(input, window);
            updateMouseInput(input, renderer);

            AppState next = state;
            switch (state) {
                case AppState::LOADING:  next = updateLoading(loading, dt);                   break;
                case AppState::MENU:     next = updateMenu(menu, input);                       break;
                case AppState::SETTINGS: next = updateSettings(settings, input, settingsData);  break;
                case AppState::GAME:     next = updateGame(game, input, dt);                   break;
                case AppState::PAUSE:    next = updatePause(pause, input);                     break;
                default: break;
            }

            if (next != state) {
                if (needsFade(state, next)) {
                    startTransition(transition, next, state);
                } else {
                    initNewState(next, state, game, menu, pause, settings, window);
                    if (!window.isOpen()) break;
                    state = next;
                }
            }

            bool isHovering = (state == AppState::MENU     && menu.isHovering)
                           || (state == AppState::PAUSE    && pause.isHovering)
                           || (state == AppState::SETTINGS && settings.isHovering);
            if (hasCursors && isHovering != prevHovering) {
                window.setMouseCursor(isHovering ? *handOpt : *arrowOpt);
                prevHovering = isHovering;
            }
        }
        else
        {
            AppState switched;
            if (stepTransition(transition, dt, switched)) {
                state = switched;
                initNewState(state, transition.from, game, menu, pause, settings, window);
                if (!window.isOpen()) break;
            }
        }

        if (fpsTimer >= 1.0f) {
            window.setTitle("Platformer | FPS: " + std::to_string(fpsCounter));
            fpsTimer = 0.f; fpsCounter = 0;
        }

        window.clear();
        switch (state) {
            case AppState::LOADING:  drawLoading(renderer, loading);                  break;
            case AppState::MENU:     drawMenu(renderer, menu);                         break;
            case AppState::SETTINGS: drawSettings(renderer, settings, settingsData);   break;
            case AppState::GAME:     drawGame(renderer, game);                         break;
            case AppState::PAUSE:
                drawGame(renderer, game);
                drawPause(renderer, pause);
                break;
            default: break;
        }

        if (transition.active) {
            uint8_t a = fadeAlpha(transition);
            if (a > 0) {
                setUIView(renderer);
                drawRect(renderer, 0.f, 0.f, (float)WINDOW_W, (float)WINDOW_H, sf::Color{0, 0, 0, a});
            }
        }

        window.display();
        window.setFramerateLimit(240);
    }
}

#pragma once
#include "core/app_state.h"
#include <cstdint>

struct Transition {
    float    t         = 0.f;
    float    duration  = 0.25f;
    AppState pending   = AppState::LOADING;
    AppState from      = AppState::LOADING;
    bool     active    = false;
    bool     fadingOut = true;
};

bool needsFade(AppState from, AppState to);

void startTransition(Transition& tr, AppState to, AppState from);

uint8_t fadeAlpha(const Transition& tr);

bool stepTransition(Transition& tr, float dt, AppState& stateOut);

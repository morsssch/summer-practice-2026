#pragma once
#include "core/app_state.h"

struct Renderer;

struct LoadingState {
  float timer = 0.f;
};

AppState updateLoading(LoadingState &s, float dt);
void drawLoading(Renderer &r, const LoadingState &s);
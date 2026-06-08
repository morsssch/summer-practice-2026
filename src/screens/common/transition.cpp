#include "transition.h"
#include <algorithm>

bool needsFade(AppState from, AppState to) {
  if (from == AppState::GAME && to == AppState::PAUSE)
    return false;
  if (from == AppState::PAUSE && to == AppState::GAME)
    return false;
  return true;
}

void startTransition(Transition &tr, AppState to, AppState from) {
  tr.active = true;
  tr.fadingOut = true;
  tr.t = 0.f;
  tr.pending = to;
  tr.from = from;
}

uint8_t fadeAlpha(const Transition &tr) {
  float t = std::clamp(tr.t, 0.f, 1.f);
  float a = tr.fadingOut ? (t * t) : ((1.f - t) * (1.f - t));
  return static_cast<uint8_t>(a * 255.f);
}

bool stepTransition(Transition &tr, float dt, AppState &stateOut) {
  tr.t += dt / tr.duration;
  if (tr.fadingOut) {
    if (tr.t >= 1.f) {
      tr.t = 0.f;
      tr.fadingOut = false;
      stateOut = tr.pending;
      return true;
    }
  } else {
    if (tr.t >= 1.f) {
      tr.t = 1.f;
      tr.active = false;
    }
  }
  return false;
}

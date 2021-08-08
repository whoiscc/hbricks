//! Internal state layout of `GameState` of the `Game`
//! Extracted from `Game.c` to make `Game` testable
#pragma once

#include "Behavior/Game.h"

struct GameState {
  const struct GameConfig *config;

  double bracket_x;
  double bracket_vx;
  double ball_x;
  double ball_y;
  double ball_vx;
  double ball_vy;
};

STATIC_ASSERT(state_size, sizeof(struct GameState) <= SIZEOF_GameState);

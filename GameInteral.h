//! Internal state layout of `GameState` of the `Game`
//! Extracted from `Game.c` to make `Game` testable
#pragma once

#include "Game.h"

struct GameState {
  double bracket_x;
  double bracket_vx;
  double ball_x;
  double ball_y;
  double ball_vx;
  double ball_vy;

  const struct GameConfig *config;
};

// *the Game*, which probably will be the only implementation of `GameBehaviour` in the project
// but maybe someday there will be a VeryHardGame, who knows
#pragma once
#include "Polyfill.h"
#include "Behavior/Game.h"

#define SIZEOF_GameState 64
extern const struct GameBehavior Game;

struct GameConfig {
  int screen_width;
  int screen_height;
  int brick_width;
  int brick_height;
  int nb_brick_col;
  int bracket_width;
  int bracket_height;
  int ball_radius;
  double speed;
  double bracket_speed;
};
extern const struct GameConfig default_game_config;

// the lifetime of `GameConfig` must outlive `GameState`
struct GameState *InitGame(uint8_t *mem, const struct GameConfig *config);
//
void DropGame(struct GameState *state);

//! Game.h: Abstract polymorphic Game definition. The default implementation of Game is appended.
//!
//! The `Game` model works on pretty low level, maintains states for stage details like animation. This enables the
//! simplification of platform-dependent `View`s, which only need to expose stateless entity-drawing interfaces.
//!
//! The disadvantage of "One-Big-Game" is that it could result in a unmaintainable Game implementation. For that the
//! Game may need to be decoupled when it grows larger.
#pragma once
#include "Tweak.h"
#include "Behavior/View.h"

struct GameState;

struct GameBehavior {
  // can only be called when `!IsOver`
  // todo determine redraw area
  void (*NextTick)(struct GameState *);
  //
  int (*IsOver)(const struct GameState *);
  // the x position of bracket is probably the only property that directly responds to user input
  // if user can do more thing, maybe it is better to convert it into a more general form
  void (*SetBracketX)(struct GameState *, double);
  //
  void (*Draw)(const struct GameState *, struct ViewState *, const struct ViewBehavior *);
};

// *the Game*, which probably will be the only implementation of `GameBehaviour` in the project
// but maybe someday there will be a VeryHardGame, who knows
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
};
extern const struct GameConfig default_game_config;

// the lifetime of `GameConfig` must outlive `GameState`
struct GameState *InitGame(uint8_t *mem, const struct GameConfig *config);
//
void DropGame(struct GameState *state);

//! Game.h: Abstract polymorphic Game definition.
//!
//! Every user of Game should take two arguments together: a pointer of `GameState` and a pointer of `GameBehavior`.
//! The structure should be like:
//! ```
//! OnUserInput(/*...*/, struct GameState *state, const struct GameBehavior *Game) {
//!   // calculate `bracket_x` based on user input
//!   Game->SetBracketX(state, bracket_x);
//! }
//! ```
//!
//! The default implementation of Game is appended as well.
#pragma once

struct GameState;

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

struct GameBehavior {
  // the lifetime of `GameConfig` must outlive `GameState`
  void (*Init)(struct GameState *, const struct GameConfig *);

  void (*Drop)(struct GameState *);

  // can only be called when `!IsOver`
  void (*NextTick)(struct GameState *);

  int (*IsOver)(const struct GameState *);

  // the x position of bracket is probably the only property that directly responds to user input
  // if user can do more thing, maybe it is better to convert it into a more general form
  void (*SetBracketX)(struct GameState *, double);
};

// *the Game*, which probably will be the only implementation of `GameBehaviour` in the project
// but maybe someday there will be a VeryHardGame, who knows
#define SIZEOF_GameState 64
extern struct GameBehavior Game;

//! Game.h: Abstract polymorphic Game definition.
//!
//! The `Game` model works on pretty low level, maintains states for stage details like animation. This enables the
//! simplification of platform-dependent `View`s, which only need to expose stateless entity-drawing interfaces.
//!
//! The disadvantage of "One-Big-Game" is that it could result in a unmaintainable Game implementation. For that the
//! Game may need to be decoupled when it grows larger.
#pragma once
#include "Lib/Polyfill.h"
#include "Lib/Behavior/View.h"

struct GameState;

struct GameBehavior {
  // can only be called when `!IsOver`
  // todo determine redraw area
  void (*NextTick)(struct GameState *);
  //
  int (*IsOver)(const struct GameState *);
  // the x position of bracket is probably the only property that directly responds to user input
  // if user can do more thing, maybe it is better to convert it into a more general form
  void (*SetBracketX)(struct GameState *, double dst, int interpolate);
  //
  void (*Draw)(const struct GameState *, struct ViewState *, const struct ViewBehavior *);

  struct ExtraBehavior Ex;
};

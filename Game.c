#include "Game.h"
#include "GameInteral.h"

#include <assert.h>
#include <math.h>

static inline int BracketTop(const struct GameState *state) {
  return state->config->screen_height - state->config->bracket_height;
}

static inline void NormalizeBallV(struct GameState *state, double k) {
  k /= hypot(state->ball_vx, state->ball_vy);
  state->ball_vx *= k;
  state->ball_vy *= k;
}

static void Init(struct GameState *state, const struct GameConfig *config) {
  assert(sizeof(struct GameState) <= SIZEOF_GameState);

  state->config = config;

  state->bracket_x = config->screen_width / 2.;
  state->ball_x = state->bracket_x;
  state->ball_y = BracketTop(state) - config->ball_radius;
  state->ball_vx = 1;
  state->ball_vy = -1;
  NormalizeBallV(state, config->speed);
}

static void Drop(struct GameState *state) {
  //
}

static int IsOver(const struct GameState *state) {
  return state->ball_y > state->config->screen_height;
}

static void SetBracketX(struct GameState *state, double x) {
  if (x < state->bracket_x / 2)
    x = state->bracket_x / 2;
  if (x > state->config->screen_width - state->bracket_x / 2)
    x = state->config->screen_width - state->bracket_x / 2;
  double old_x = state->bracket_x;
  state->bracket_x = x;
  // todo
  state->bracket_vx = x - old_x;
}

// if hit, which probably means overlapping, is detected, the ball is immediately taken out of the other entity
// to prevent false positive hit after the actual hit is processed in previous tick

// `HitWall` may return 2 when the ball hit a corner exactly, it should be impossible for `HitWall` to return 3
// in the future this feature may be useful to implement hit counter
static int HitWall(struct GameState *state) {
  int hit = 0;
  if (state->ball_x <= state->config->ball_radius) {
    state->ball_x = state->config->ball_radius;
    state->ball_vx = -state->ball_vx;
    hit += 1;
  }
  if (state->ball_x >= state->config->screen_width - state->config->ball_radius) {
    state->ball_x = state->config->screen_width - state->config->ball_radius;
    state->ball_vx = -state->ball_vx;
    hit += 1;
  }
  if (state->ball_y <= state->config->ball_radius) {
    state->ball_y = state->config->ball_radius;
    state->ball_vy = -state->ball_vy;
    hit += 1;
  }
  return hit;
}

static int HitBracket(struct GameState *state) {
  int check_x = state->ball_x >= state->bracket_x - state->config->bracket_width / 2.
      && state->ball_x <= state->bracket_x + state->config->bracket_width / 2.;
  int check_y = state->ball_y >= BracketTop(state) - state->config->ball_radius;
  if (check_x && check_y) {
    state->ball_y = BracketTop(state) - state->config->ball_radius;
    state->ball_vy = -state->ball_vy;
    // x velocity contains three components: original `ball_vx`, current `bracket_vx` which simulates friction,
    // and the distance between the origin of ball and the origin of bracket, which follows the classical Breakout
    // game, and should be fun to include
    // todo the multiplier of each component should be finely tuned
    state->ball_vx = 0.7 * state->ball_vx + 0.2 * state->bracket_vx + 0.1 * (state->ball_x - state->bracket_x);
    NormalizeBallV(state, state->config->speed);
    return 1;
  }
  // todo hit bracket side
  return 0;
}

static void NextTick(struct GameState *state) {
  assert(!Game.IsOver(state));

  // in rare cases the ball could hit more than one entity, e.g., both wall and bracket, at the same time, i.e. in the
  // same `NextTick` calling
  // to simplify code only one hit is processed in one tick, with unintentional priority
  // since all hit handler can tolerance overlapping to some extent, this should be fine
  // hopefully the ball is not too fast to make it looks weird
  do {
    if (HitWall(state))
      break;
    if (HitBracket(state))
      break;
    // todo hit brick
  } while (0);

  state->ball_x += state->ball_vx;
  state->ball_y += state->ball_vy;
}

struct GameBehavior Game = {
    .Init = Init,
    .Drop = Drop,
    .NextTick = NextTick,
    .IsOver = IsOver,
    .SetBracketX = SetBracketX,
};

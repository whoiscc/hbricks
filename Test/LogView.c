#define IMPL_LogView
#include "Test/LogView.h"

#include <assert.h>
#include <stdint.h>

struct ViewState *InitLogView(uint8_t *mem) {
  struct ViewState *state = (struct ViewState *) mem;
  state->n = 0;
  return state;
}

static void AddBall(struct ViewState *state, double x, double y, int r) {
  assert(state->n < NB_LogMax);
  state->log[state->n].kind = ADD_BALL;
  state->log[state->n].add_ball.x = x;
  state->log[state->n].add_ball.y = y;
  state->log[state->n].add_ball.r = r;
  state->n += 1;
}

static void AddBracket(struct ViewState *state, double x, double y, int w, int h) {
  assert(state->n < NB_LogMax);
  state->log[state->n].kind = ADD_BRACKET;
  state->log[state->n].add_bracket.x = x;
  state->log[state->n].add_bracket.y = y;
  state->log[state->n].add_bracket.w = w;
  state->log[state->n].add_bracket.h = h;
  state->n += 1;
}

const struct ViewBehavior LogView = {
    .AddBall = AddBall,
    .AddBracket = AddBracket,
};

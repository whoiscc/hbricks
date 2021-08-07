#include "Platform/Text/View.h"

#include <stdio.h>

struct ViewState {
  int _[0];
};

struct ViewState *InitTextView(uint8_t *mem) {
  return (struct ViewState *) mem;
}

static void AddBall(struct ViewState *state, double x, double y, int r) {
  printf("Ball is at {x = %f, y = %f}.\n", x, y);
  // todo hint radius changing
}

static void AddBracket(struct ViewState *state, double x, double y, int w, int h) {
  printf("Bracket is at {x = %f}.\n", x);
}

const struct ViewBehavior TextView = {
    .AddBall = AddBall,
    .AddBracket = AddBracket,
};
//! Views/Log.h: A mock implementation for View.
//!
//! Because `LogView` is designed for debugging and testing, the state layout is public.
#pragma once
#include "View.h"

#include <stdint.h>

#define NB_LogMax 16

struct ViewState {
  struct Log {
    enum Kind {
      KIND_LogView_AddBall,
      KIND_LogView_AddBracket,
    } kind;
    union {
      struct {
        double x;
        double y;
        int r;
      } add_ball;
      struct {
        double x;
        double y;
        int w;
        int h;
      } add_bracket;
    };
  } log[NB_LogMax];
  int n;
};

#define SIZEOF_LogViewState sizeof(struct ViewState)
const extern struct ViewBehavior LogView;

struct ViewState *InitLogView(uint8_t *mem);

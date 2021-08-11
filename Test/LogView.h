//! Views/Log.h: A mock implementation for View.
//!
//! Because `LogView` is designed for debugging and testing, the state layout is public.
#pragma once
#include "Lib/Polyfill.h"
#if !IS_IMPL(LogView) && !defined(__cplusplus)
#error "LogView is not designed for using in C"
#endif
#include "Lib/Behavior/View.h"

#define NB_LogMax 16

struct ViewState {
  struct Log {
    enum Kind {
      ADD_BALL,
      ADD_BRACKET,
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

#if !IS_IMPL(LogView)
#undef NB_LogMax
#endif

#define SIZEOF_LogViewState sizeof(struct ViewState)
const extern struct ViewBehavior LogView;

struct ViewState *InitLogView(uint8_t *mem);

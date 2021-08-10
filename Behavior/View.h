#pragma once
#include "Behavior/Extra.h"

struct ViewState;

struct ViewBehavior {
  //
  void (*AddBall)(struct ViewState *, double x, double y, int r);
  //
  void (*AddBracket)(struct ViewState *, double x, double y, int w, int h);
  //
  struct Extra Ex;
};

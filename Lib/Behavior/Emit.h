#pragma once
#include "Lib/Polyfill.h"

struct EmitState;

struct EmitBehavior {
  // indicate a rect area that will be drawn differently in the next `Draw` call to the previous one
  // normally the invalidated rects are paired, for each entity that is moving, both rect of its previous and next
  // location need to be invalidated
  // usually the two rects overlaps, but that is not necessary
  // `Game` will not union the rects in that case, because these are frame-based interfaces, so there is no more
  // sub-interval for transition. (although reasonable this case should not happen still)
  //
  // some platform may support invalidating other shapes natively, but because this is a brick game, so most of the
  // invalidation should be rect-shaped
  //
  // `Game` promises that every pixel that is out of all `Invalidate` will be drawn identically as before
  // but a `Draw` call will still contain drawing spec for the whole screen area
  // so the `View` can still naively clear the whole screen on every frame, and optionally, it can skip unnecessary
  // drawing base to invalidation status
  //
  // it is a little insane to require sub-pixel invalidation, so the rect is rounded up to pixel boundary
  void (*Invalidate)(struct EmitState *, int min_x, int min_y, int max_x, int max_y);
  //
  void (*HitWall)(struct EmitState *, int x, int y);
  // TODO add brick pointer? after adding brick behavior
  void (*HitBrick)(struct EmitState *, int x, int y);
  //
  void (*HitBracket)(struct EmitState *, int x, int y);
  //
  struct Extra Ex;
};

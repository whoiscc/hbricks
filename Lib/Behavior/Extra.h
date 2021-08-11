#pragma once
// do not rely on polyfill, because itself is part of polyfill

struct StrBufState;

struct ExtraBehavior {
  void (*Repr)(void *, struct StrBufState *);
};
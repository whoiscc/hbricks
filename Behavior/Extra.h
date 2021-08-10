#pragma once
// no need to include polyfill for now

struct Extra {
  void (*Repr)(void *, struct StrBufState *);
};
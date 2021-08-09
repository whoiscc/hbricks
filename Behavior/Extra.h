#pragma once
#include "Polyfill.h"

struct Extra {
  void (*Repr)(void *, struct StrBufState *);
};
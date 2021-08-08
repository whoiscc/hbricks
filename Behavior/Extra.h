#pragma once
#include "Polyfill.h"

struct ReprState;

struct Extra {
  void (*Repr)(struct ReprState *, struct StrBuf *);
};
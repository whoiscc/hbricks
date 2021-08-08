#pragma once
#include "Polyfill.h"


struct DebugFmtState;

struct Extra {
  void (*DebugFmt)(struct DebugFmtState *, struct StrBuf *);
};
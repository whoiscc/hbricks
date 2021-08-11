#pragma once
#include "Lib/Polyfill.h"

struct StrBufState;
#define SIZEOF_StrBufState 32

extern NAMESPACE(StrBuf) {
  //
  struct StrBufState *(*Init)(uint8_t *mem);
  //
  void (*Drop)(struct StrBufState *str_buf);
  //
  void (*Append)(struct StrBufState *, const char *);
  //
  void (*FmtWrite)(struct StrBufState *, const char *, ...);
  //
  const char *(*View)(const struct StrBufState *);
  //
  const struct ExtraBehavior Ex;
} StrBuf;

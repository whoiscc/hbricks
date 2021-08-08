#pragma once
#include "Polyfill.h"

struct StrBufState;
#define SIZEOF_StrBufState 32

//
struct StrBufState *InitStrBuf(uint8_t *mem);
//
extern const struct StrBuf {
  void (*Append)(struct StrBufState *, const char *);
  const char *(*View)(const struct StrBufState *);
} StrBuf;
//
void DropStrBuf(struct StrBufState *str_buf);
//

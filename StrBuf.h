#pragma once
#include "Polyfill.h"
#include "Behavior/Extra.h"

struct StrBufState;
#define SIZEOF_StrBufState 32

//
struct StrBufState *InitStrBuf(uint8_t *mem);
//
extern NAMESPACE(StrBuf) {
  void (*Append)(struct StrBufState *, const char *);
  void (*FmtWrite)(struct StrBufState *, const char *, ...);
  const char *(*View)(const struct StrBufState *);
  const struct Extra Ex;
} StrBuf;
//
void DropStrBuf(struct StrBufState *str_buf);
//

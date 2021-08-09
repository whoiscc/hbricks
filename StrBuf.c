#define IMPL_StrBuf
#include "StrBuf.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define SIZEOF_embedded (SIZEOF_StrBufState - sizeof(size_t))

struct StrBufState {
  // `cap` == 0 when content is embedded
  // `cap` includes trailing '\0', so maximum length of string is `cap - 1`
  size_t cap;
  union {
    char *ptr;
    char embedded[SIZEOF_embedded];
  };
};

STATIC_ASSERT(state_size, sizeof(struct StrBufState) <= SIZEOF_StrBufState);

struct StrBufState *InitStrBuf(uint8_t *mem) {
  struct StrBufState *str_buf = (struct StrBufState *) mem;
  str_buf->cap = 0;
  str_buf->ptr = NULL;  // embedded[0..<8] = 0
  return str_buf;
}

static inline int IsEmbedded(const struct StrBufState *this) {
  assert(this->cap == 0 || this->cap >= SIZEOF_embedded);
  return this->cap == 0;
}

void DropStrBuf(struct StrBufState *this) {
  if (!IsEmbedded(this)) {
    free(this->ptr);
  }
}

static inline size_t GetLen(const struct StrBufState *this) {
  return IsEmbedded(this) ? strlen(this->embedded) : this->cap;
}

STATIC_ASSERT(one_byte_char, sizeof(char) == 1);

static void Append(struct StrBufState *this, const char *str) {
  size_t len = GetLen(this) + strlen(str);  // `len` is without '\0'
  if (IsEmbedded(this) && len < SIZEOF_embedded) {
    strcpy_s(this->embedded + GetLen(this), SIZEOF_embedded - GetLen(this), str);
    return;
  }

  char *ptr;
  size_t next_size = len + 1;  // 1 for '\0'
  if (IsEmbedded(this)) {
    ptr = malloc(next_size);
    strcpy_s(ptr, next_size, this->embedded);
  } else {
    ptr = realloc(this->ptr, next_size);
  }
  strcpy_s(ptr + GetLen(this), next_size - GetLen(this), str);
  this->ptr = ptr;
  this->cap = len;
}

void FmtWrite(struct StrBufState *this, const char *fmt, ...) {
  // there is a `vasprintf` on Linux, but not on Windows
  // it is undesirable to polyfill that function, since the whole StrBuf is already a polyfill
  // some performance may be lost here, just note for it
  va_list args, args_cloned;
      va_start(args, fmt);
  va_copy(args_cloned, args);
  int len = vsnprintf(NULL, 0, fmt, args);
      va_end(args);
  char *buf = malloc(len + 1);
  vsnprintf(buf, len + 1, fmt, args_cloned);
      va_end(args_cloned);
  StrBuf.Append(this, buf);
  free(buf);
}

static const char *View(const struct StrBufState *this) {
  return IsEmbedded(this) ? this->embedded : this->ptr;  // notice: they are NOT the same
}

static void Repr(void *repr, struct StrBufState *out) {
  struct StrBufState *this = repr;
  StrBuf.FmtWrite(out,
                  "StrBuf%s{len=%zu,view=%.10s%s}",
                  IsEmbedded(this) ? "[emb]" : "",
                  GetLen(this),
                  StrBuf.View(this),
                  GetLen(this) > 10 ? "..." : "");
}

NAMESPACE(StrBuf) StrBuf = {
    .Append = Append,
    .FmtWrite = FmtWrite,
    .View = View,
    .Ex = {
        .Repr = Repr,
    },
};

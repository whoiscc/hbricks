//! Polyfill.h: Common misc code for the whole project.
//!
//! "Polyfill" is for the lack of a better name. This header is a complement of C standard library, including several
//! conventional patterns that should be utilized consistent across the codebase.
//! Every code file should include this header.
#pragma once
#include <stdint.h>

#include "StrBuf.h"

// MSVC not allow to typedef zero-length array type
#define STATIC_ASSERT(id, expr) \
  typedef int HBricksInternal_StaticAssert_##id[(expr) ? 42 : -1]

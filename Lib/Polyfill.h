//! Polyfill.h: Common misc code for the whole project.
//!
//! "Polyfill" is for the lack of a better name. This header is a complement of C standard library, including several
//! conventional patterns that should be utilized consistent across the codebase.
//! Every code file should include this header.
#pragma once
#include <stdint.h>

// https://stackoverflow.com/a/49712473
// minor tweak: __ARG_PLACEHOLDER_1 -> __ARG_PLACEHOLDER, because our defined macro has no value
#define __ARG_PLACEHOLDER 0,
#define __take_second_arg(__ignored, val, ...) val
#define __is_defined(x)         ___is_defined(x)
#define ___is_defined(val)      ____is_defined(__ARG_PLACEHOLDER##val)
#define ____is_defined(arg1_or_junk)    __take_second_arg(arg1_or_junk 1, 0)

#define IS_IMPL(name) __is_defined(IMPL_##name)

// it is achievable to only expose struct type name to implementation source
// however since it is already `HBricksInternal` it does not feel like necessary
#define NAMESPACE(name) const struct HBricksInternal_Namespace_##name

// MSVC not allow to typedef zero-length array type
#define STATIC_ASSERT(id, expr) \
  typedef int HBricksInternal_StaticAssert_##id[(expr) ? 42 : -1]

// Utilities, e.g. common structure and container types
// Normally these headers include this polyfill, which creates circle
// make sure everything they rely on goes above, and think about better solution
// Extra has to go to before all other components, most of them contain nested ExtraBehavior rather than pointer
#include "Lib/Behavior/Extra.h"
#include "Lib/StrBuf.h"

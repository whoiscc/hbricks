#pragma once

#include <stdint.h>

#define STATIC_ASSERT_SIZEOF(impl, behavior) \
  typedef int HBricksInternalStaticAssertSizeOf[SIZEOF_##impl##behavior##State - sizeof(struct behavior##State)]

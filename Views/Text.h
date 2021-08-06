#pragma once
#include "View.h"

#include <stdint.h>

#define SIZEOF_TextViewState 64
extern const struct ViewBehavior TextView;

struct ViewState *InitTextView(uint8_t *mem);

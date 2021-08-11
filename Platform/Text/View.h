#pragma once
#include "Behavior/View.h"

#include "Lib/Polyfill.h"

#define SIZEOF_TextViewState 32
extern const struct ViewBehavior TextView;

struct ViewState *InitTextView(uint8_t *mem);

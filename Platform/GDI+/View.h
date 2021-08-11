#pragma once
extern "C" {
#include "Behavior/View.h"

#include "Lib/Polyfill.h"
}

#include <windows.h>

#define SIZEOF_GDIPlusViewState 32
extern const struct ViewBehavior GDIPlusView;

class GDIPlusGuard {
  ULONG_PTR token;
public:
  GDIPlusGuard();
  ~GDIPlusGuard();
};
//
struct ViewState *InitGDIPlusView(uint8_t *mem, HDC hdc);
//
void DropGDIPlusView(struct ViewState *state);

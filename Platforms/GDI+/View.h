#pragma once
#include "Behavior/View.h"

#include <cstdint>

#include <Windows.h>

#define SIZEOF_GDIPlusViewState 64
extern const struct ViewBehavior GDIPlusView;

class GDIPlusGuard {
  ULONG_PTR token;
public:
  GDIPlusGuard();
  ~GDIPlusGuard();
};
//
struct ViewState *InitGDIPlusView(std::uint8_t *mem, HDC hdc);
//
void DropGDIPlusView(struct ViewState *state);

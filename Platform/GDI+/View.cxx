#include "Platform/GDI+/View.h"

#include <cassert>
#include <new>

#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;

GDIPlusGuard::GDIPlusGuard() : token() {
  GdiplusStartupInput input;
  GdiplusStartup(&token, &input, nullptr);
}

GDIPlusGuard::~GDIPlusGuard() {
  GdiplusShutdown(token);
}

struct ViewState {
  Graphics graphics;
};

struct ViewState *InitGDIPlusView(uint8_t *mem, HDC hdc) {
  assert(sizeof(struct ViewState) <= SIZEOF_GDIPlusViewState);
  auto *state = reinterpret_cast<struct ViewState *>(mem);
  ::new(&state->graphics) Graphics(hdc);
  state->graphics.Clear(Color::White);  // todo only clear redraw area
  state->graphics.SetSmoothingMode(SmoothingModeAntiAlias);
  return state;
}

void DropGDIPlusView(struct ViewState *state) {
  state->graphics.~Graphics();
}

static void AddBall(struct ViewState *state, double x, double y, int r) {
  SolidBrush brush(Color::Gray);  // todo texture ball
  REAL ellipse_x = REAL(x - r);
  REAL ellipse_y = REAL(y - r);
  state->graphics.FillEllipse(&brush, ellipse_x, ellipse_y, REAL(r * 2), REAL(r * 2));
}

static void AddBracket(struct ViewState *state, double x, double y, int w, int h) {
  SolidBrush brush(Color::Black);  // todo texture bracket
  REAL rect_x = REAL(x - w / 2.);
  REAL rect_y = REAL(y - h / 2.);
  state->graphics.FillRectangle(&brush, rect_x, rect_y, REAL(w), REAL(h));
}

const struct ViewBehavior GDIPlusView = {
    AddBall,
    AddBracket,
};

#include "Views/GDI+.h"

#include <cassert>

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
  Graphics *graphics;
};

struct ViewState *InitGDIPlusView(uint8_t *mem, HDC hdc) {
  assert(sizeof(struct ViewState) <= SIZEOF_GDIPlusViewState);
  auto *state = reinterpret_cast<struct ViewState *>(mem);
  state->graphics = new Graphics(hdc);
  state->graphics->Clear(Color::White);  // todo only clear redraw area
  return state;
}

void DropGDIPlusView(struct ViewState *state) {
  delete state->graphics;
}

static void AddBall(struct ViewState *state, double x, double y, int r) {
  SolidBrush brush(Color::Gray);  // todo texture ball
  state->graphics->FillEllipse(&brush, REAL(x), REAL(y), REAL(r), REAL(r));
}

static void AddBracket(struct ViewState *state, double x, double y, int w, int h) {
  SolidBrush brush(Color::Black);  // todo texture bracket
  REAL rect_x = REAL(x - w / 2.);
  REAL rect_y = REAL(y - h / 2.);
  state->graphics->FillRectangle(&brush, rect_x, rect_y, REAL(w), REAL(h));
}

const struct ViewBehavior GDIPlusView = {
    .AddBall = AddBall,
    .AddBracket = AddBracket,
};

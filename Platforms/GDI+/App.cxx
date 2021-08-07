#ifndef UNICODE
#define UNICODE
#endif

#include <cstdint>

#include <Windows.h>

extern "C" {
#include "Behavior/Game.h"
};
#include "Platforms/GDI+/View.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct WindowParam {
  struct GameState *game;
  const struct GameBehavior *Game;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  // Register the window class.
  const wchar_t CLASS_NAME[] = L"hbricks";

  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  std::uint8_t game_mem[SIZEOF_GameState];
  struct GameConfig config = default_game_config;
  struct GameState *game = InitGame(game_mem, &config);

  GDIPlusGuard g;

  // Create the window.
  RECT rect = {0, 0, config.screen_width, config.screen_height};
  AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW, false, 0, GetDpiForWindow(GetDesktopWindow()));

  WindowParam param = {game, &Game};
  HWND hwnd = CreateWindowEx(
      0,                              // Optional window styles.
      CLASS_NAME,                     // Window class
      L"Humor Bricks",                // Window text
      WS_OVERLAPPEDWINDOW,            // Window style

      // Size and position
      CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,

      nullptr,       // Parent window
      nullptr,       // Menu
      hInstance,     // Instance handle
      &param         // Additional application data
  );

  if (hwnd == nullptr) {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  // Run the message loop.
  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  DropGame(game);
  return (int) msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CREATE: {
    auto create = reinterpret_cast<CREATESTRUCT *>(lParam);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) create->lpCreateParams);
    return 0;
  }

  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }

  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    // https://blogs.windows.com/windowsdeveloper/2017/05/19/improving-high-dpi-experience-gdi-based-desktop-apps/
    RECT rect;
    GetClientRect(hwnd, &rect);
    int cx = rect.right - rect.left;
    int cy = rect.bottom - rect.top;
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, cx, cy);
    HDC memDC = CreateCompatibleDC(hdc);
    auto oldBitmap = (HBITMAP) SelectObject(memDC, memBitmap);

    std::uint8_t view_mem[SIZEOF_GDIPlusViewState];
    struct ViewState *view = InitGDIPlusView(view_mem, memDC);
    auto param = reinterpret_cast<WindowParam *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    param->Game->Draw(param->game, view, &GDIPlusView);
    DropGDIPlusView(view);

    BitBlt(hdc, 0, 0, cx, cy, memDC, 0, 0, SRCCOPY);
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    EndPaint(hwnd, &ps);
    return 0;
  }

  default: { break; }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

VOID OnPaint(HDC hdc) {
  Graphics graphics(hdc);
  Pen pen(Color(255, 0, 0, 255));
  graphics.DrawLine(&pen, 0, 0, 200, 100);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  // Register the window class.
  const wchar_t CLASS_NAME[] = L"hbricks";

  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  // Initialize GDI+.
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

  // Create the window.
  HWND hwnd = CreateWindowEx(
      0,                              // Optional window styles.
      CLASS_NAME,                     // Window class
      L"Humor Bricks",                // Window text
      WS_OVERLAPPEDWINDOW,            // Window style

      // Size and position
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

      nullptr,       // Parent window
      nullptr,       // Menu
      hInstance,     // Instance handle
      nullptr        // Additional application data
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

  GdiplusShutdown(gdiplusToken);
  return (int) msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_DESTROY: {
    PostQuitMessage(0);
    return 0;
  }

  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
    OnPaint(hdc);

    EndPaint(hwnd, &ps);
    return 0;
  }

  default: { break; }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
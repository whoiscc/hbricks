#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

VOID OnPaint(HDC hdc) {
  Graphics graphics(hdc);
  graphics.Clear(Color::White);

  SolidBrush brush(Color(255, 0, 0, 0));
  FontFamily fontFamily(L"Calibri");
  Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
  PointF pointF(10.0f, 20.0f);
  graphics.DrawString(L"Hello World!", -1, &font, pointF, &brush);
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

    RECT rect;
    GetClientRect(hwnd, &rect);

    UINT cx = (rect.right - rect.left);
    UINT cy = (rect.bottom - rect.top);

    // Create a compatible bitmap using paint DC.
    // Compatible bitmap will be properly scaled in size internally and
    // transparently to the app to match current monitor DPI where
    // the window is located.
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, cx, cy);

    // Create a compatible DC, even without a bitmap selected,
    // compatible DC will inherit the paint DC GDI scaling
    // matching the window monitor DPI.
    HDC memDC = CreateCompatibleDC(hdc);

    // Selecting GDI scaled compatible bitmap in the
    // GDI scaled compatible DC.
    auto oldBitmap = (HBITMAP) SelectObject(memDC, memBitmap);

    // Drawing content on the compatible GDI scaled DC.
    // If the monitor DPI was 150% or 200%, text internally will
    // be drawn at next integral scaling value, in current example
    // 200%.
    OnPaint(memDC);

    // Copying the content back from compatible DC to paint DC.
    // Since both compatible DC and paint DC are GDI scaled,
    // content is copied without any stretching thus preserving
    // the quality of the rendering.
    BitBlt(hdc, 0, 0, cx, cy, memDC, 0, 0, SRCCOPY);

    // Cleanup.
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    // At this time the content is presented to the screen.
    // DWM (Desktop Window Manager) will scale down if required the
    // content to actual monitor DPI.
    // If the monitor DPI is already an integral one, for example 200%,
    // there would be no DWM down scaling.
    // If the monitor DPI is 150%, DWM will scale down rendered content
    // from 200% to 150%.
    // While not a perfect solution, it’s better to scale-down content
    // instead of scaling-up since a lot of the details will be preserved
    // during scale-down.
    // The end result is that with GDI Scaling enabled, the content will
    // look less blurry on screen and in case of monitors with DPI setting
    // is set to an integral value (200%, 300%) the vector based and text
    // content will be rendered natively at the monitor DPI looking crisp
    // on screen.
    EndPaint(hwnd, &ps);
    return 0;
  }

  default: { break; }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
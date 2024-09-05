#include "appWindow.h"
#include <codecvt>

appWindow::appWindowClass appWindow::appWindowClass::winClass;

appWindow::appWindowClass::appWindowClass()
  :
  hInst ( GetModuleHandle (nullptr))
{
  WNDCLASSEX wc = {};
  //Define WinClass
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MessageHandlerSetup;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetInstance();
  wc.hIcon = LoadIcon(wc.hInstance, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = GetName();
  wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

  //Register our window with the WindowsOS
  RegisterClassEx(&wc);
}

appWindow::appWindowClass::~appWindowClass()
{
  UnregisterClass(winClass.GetName(), winClass.GetInstance());
}

LPCWSTR appWindow::appWindowClass::GetName()
{
  return winClassName;
}
HINSTANCE appWindow::appWindowClass::GetInstance()
{
  return winClass.hInst;
}

std::optional<int> appWindow::ProcessMessages()
{
  MSG msg;
  // while queue has messages, remove and dispatch them (but do not block on empty queue)
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    // check for quit because peekmessage does not signal this via return val
    if (msg.message == WM_QUIT)
    {
      // return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
      return (int)msg.wParam;
    }
    // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return {};
}

Graphics& appWindow::Gfx()
{
  return *pGfx;
};

LRESULT CALLBACK appWindow::MessageHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_NCCREATE)
  {
    // extract ptr to window class from creation data
    const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
    appWindow* const pWnd = static_cast<appWindow*>(pCreate->lpCreateParams);
    // set WinAPI-managed user data to store ptr to window instance
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    // set message proc to normal (non-setup) handler now that setup is finished
    SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&appWindow::MessageHandlerForward));
    // forward message to window instance handler
    return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
  }
  // if we get a message before the WM_NCCREATE message, handle with default handler
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK appWindow::MessageHandlerForward(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // retrieve ptr to window instance
  appWindow* const pWnd = reinterpret_cast<appWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  // forward message to window instance handler
  return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
}

LRESULT appWindow::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_KEYDOWN:
    if (wParam == 'F')
    {
      PostMessage(hWnd, WM_CLOSE, wParam, lParam);
      break;
    }
  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
}

appWindow::PrimaryDisplay appWindow::PrimaryDisplay::mainMonitor;

appWindow::PrimaryDisplay::PrimaryDisplay()
  :
  primaryDisplay(PrimaryDisplay::GetPrimaryMonitorHandle())
{
  RECT rcMonitor{};
  RECT rcWork{};
  tagMONITORINFO myPrimaryDisplay = { sizeof(MONITORINFO), rcMonitor, rcWork, MONITORINFOF_PRIMARY };
  tagMONITORINFO* pmyPrimaryDisplay = &myPrimaryDisplay;
  GetMonitorInfoA(primaryDisplay, pmyPrimaryDisplay);

  displayW = pmyPrimaryDisplay->rcMonitor.right  - pmyPrimaryDisplay->rcMonitor.left;
  displayH = pmyPrimaryDisplay->rcMonitor.bottom - pmyPrimaryDisplay->rcMonitor.top;
}

appWindow::PrimaryDisplay::~PrimaryDisplay()
{
}

HMONITOR appWindow::PrimaryDisplay::GetPrimaryMonitorHandle()
{
  const POINT ptZero = { 0, 0 };
  return MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
}

int appWindow::PrimaryDisplay::GetWidth()
{
  return mainMonitor.displayW;
}

int appWindow::PrimaryDisplay::GetHeight()
{
  return mainMonitor.displayH;
}


std::wstring CreateWindowTitle(std::wstring name, int width, int height)
{
  using namespace std;
  wstring wstrName = name + L" " + to_wstring(width) + L" " + L"x" + L" "+ to_wstring(height);
  return wstrName;
}

appWindow::appWindow(int width, int height, std::wstring appname)
  : 
  width(width), 
  height(height)
{
  //Display at the center of the screen.
  int centerAppW = (PrimaryDisplay::GetWidth() / 2) -  (width / 2);
  int centerAppH = (PrimaryDisplay::GetHeight() / 2) - (height / 2);
  std::wstring winTitle = CreateWindowTitle(appname, width, height);

  hWnd = CreateWindowEx( 
    0, appWindowClass::GetName(), (LPCWSTR)winTitle.c_str(),
    WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
    centerAppW, centerAppH, width, height, 0, 0, appWindowClass::GetInstance(), 0);

  ShowWindow(hWnd, SW_SHOW);
  pGfx = std::make_unique<Graphics>(hWnd, width, height);
  UpdateWindow(hWnd);
}

appWindow::~appWindow()
{
  DestroyWindow(hWnd);
}


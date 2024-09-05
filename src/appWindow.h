#pragma once
#include <Windows.h>
#include "aGraphics.h"
#include <memory>
#include <optional>
#include <exception>
#include <string>

class appWindow
{
private:
  class appWindowClass
  {
  public:
    static LPCWSTR GetName();
    static HINSTANCE GetInstance();
  private:
    appWindowClass();
    ~appWindowClass();
    appWindowClass(const appWindowClass&) = delete;
    static constexpr const LPCWSTR winClassName = L"Main App Class";
    static appWindowClass winClass;
    HINSTANCE hInst;
  };


private:
  class PrimaryDisplay
  {
  public:
    static int GetWidth();
    static int GetHeight();
  private:
    PrimaryDisplay();
    ~PrimaryDisplay();
    PrimaryDisplay(const PrimaryDisplay&) = delete;
    HMONITOR GetPrimaryMonitorHandle();
    static PrimaryDisplay mainMonitor;
    HMONITOR primaryDisplay;
    int displayW;
    int displayH;
  };

public:
  appWindow(int width, int height, std::wstring appname);
  ~appWindow();
  appWindow(const appWindow&) = delete;
  static std::optional<int> ProcessMessages();
  Graphics& Gfx();
private:
  static LRESULT CALLBACK MessageHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK MessageHandlerForward(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
  int width;
  int height;
  HWND hWnd;
  std::unique_ptr<Graphics> pGfx;
};
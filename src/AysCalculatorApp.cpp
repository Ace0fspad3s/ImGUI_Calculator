#include <Windows.h>
#include "App.h"
#include <sstream>

int WINAPI wWinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PWSTR pCmdLine,
  int nCmdShow)
{
  std::wstring AppName = L"Ay's App Window";
  App{AppName}.AppRun();
  return 1;
}
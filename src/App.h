#pragma once
#include "appWindow.h"
#include <string>


class App
{
public:
  App(std::wstring appname);
  int AppRun();
  ~App();
private:
  void CompletedFrame();
private:
  appWindow wnd;
};
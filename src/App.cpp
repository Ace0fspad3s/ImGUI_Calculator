#include "App.h"

App::App(std::wstring appname)
  : 
  wnd(640, 480, appname)
{
}

App::~App()
{
}
void App::CompletedFrame()
{
  wnd.Gfx().ClearBuffer(0, 0.75, 0.75);
  wnd.Gfx().DisplayFrame();
}


int App::AppRun()
{
  while (true)
  {
    //process winAPI messages
    if (const auto errCode = appWindow::ProcessMessages())
    {
      //return an error code, if any
      return *errCode;
    }
    //TODO - input Handler See appWindow::MessageHandler()
    //TODO - IMGUI implementation :)
    CompletedFrame();
  }
}

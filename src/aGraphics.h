#pragma once
// Will be managing most of the D3D stuff here
#include <d3d11.h>

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

class Graphics
{
public:
  Graphics (HWND hWnd, int width, int height);
  Graphics (const Graphics&) = delete; //Rule of three, and we don't need to move Graphics();
  Graphics& operator=(const Graphics&) = delete;
  ~Graphics();
  void DisplayFrame();
  void ClearBuffer(float red, float green, float blue) noexcept
  {
    const float color[] = { red,green,blue,1.0f };
    pContext->ClearRenderTargetView(pRenderTarget, color);
  }
private:
  UINT width = 640;
  UINT height = 480;
  Microsoft::WRL::ComPtr<ID3D11Device>        pDevice; // Our DirectX Device :)
  Microsoft::WRL::ComPtr<IDXGISwapChain>      pSwapChain;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext; //Immediate context, performs rendering
  ID3D11RenderTargetView*                     pRenderTarget;
};

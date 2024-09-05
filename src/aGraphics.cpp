#include "aGraphics.h"

D3D_DRIVER_TYPE driverTypes[] =
{
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
};

D3D_FEATURE_LEVEL featureLevels[] =
{
    D3D_FEATURE_LEVEL_11_0,
};
UINT numFeatureLevels = ARRAYSIZE(featureLevels);

Graphics::Graphics (HWND hWnd, int width, int height)
  :
  width(width),
  height(height) 
{

  //== Swapchain ==
  //DirectX 11.0
  //Swap Chains
  DXGI_SWAP_CHAIN_DESC sd{};
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  

  UINT swapCreateFlags = 0u;
  //for monitoring d3d results
  HRESULT hr;

  D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    swapCreateFlags,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &sd,
    &pSwapChain,
    &pDevice,
    nullptr,
    &pContext
  );

  //gain access to texture subresource swap chain in the Back Buffer
  ID3D11Resource* pBackBuffer = nullptr;
  pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),reinterpret_cast<void**>(&pBackBuffer));
  pDevice->CreateRenderTargetView(
    pBackBuffer,
    nullptr,
    &pRenderTarget
  );
  pBackBuffer->Release();
}

Graphics::~Graphics()
{
}

void Graphics::DisplayFrame()
{
  pSwapChain->Present(1u, 0u);
}

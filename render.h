#pragma once

#include "d3d11.h"
#include "d2d1_1.h"
#include "wrl.h"
#include "dxgi1_6.h"

#include <functional>
#include <memory>
#include <vector>

using Microsoft::WRL::ComPtr;

class Renderer
{
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<ID3D11Device> m_device;
    ComPtr<IDXGISwapChain> m_swapChain;
    DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
    ComPtr<ID3D11Texture2D> m_backBuffer;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    HWND m_hwnd;
    long m_captureWidth;
    long m_captureHeight;
    Renderer() = delete;
public:
    Renderer(long captureWidth, long captureHeight, DXGI_FORMAT);
    ComPtr<ID3D11Device> GetD3DDevice() { return m_device; }
    ComPtr<ID3D11DeviceContext> GetD3DDeviceContext() { return m_deviceContext; }
    HRESULT StartRenderLoop(std::function<HRESULT(ComPtr<ID3D11Texture2D>& texture2D)> captureFn, const double fps);
private:
    HRESULT CreateDeviceAndSwapChain();
    HRESULT SetBackground();
};
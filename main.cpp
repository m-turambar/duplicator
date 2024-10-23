#include "capturer.h"
#include "window.h"

#include <iostream>
#include <chrono>

int main()
{
    auto [captureWidth, captureHeight] = getMainDisplayDimensions();
    auto hwnd = createWindow();
    auto [d3dContext, d3dDevice, swapChain] = createDeviceAndSwapChain(hwnd, captureWidth, captureHeight);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        std::cerr << "Failed to get access to the swap chain's back buffer\n";
    }

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    hr = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create render target view\n";
    }

    d3dContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
    
    defineTheViewPort(d3dContext, captureWidth, captureHeight);

    auto duplicator = GetDuplicatorFromDevice(d3dDevice);

    auto setBackground = [&] () {
        static float r = 0.0f;
        static float g = 0.2f;
        static float b = 0.4f;
        const float clearColor[4] = {r, g, b, 1.0f};
        d3dContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
        swapChain->Present(1, 0);
        static float delta = 0.02f;
        r += delta;
        if (r > 1.0f || r < 0.0f) {
            delta *= -1;
        }
    };
    setBackground();

    auto captureFrame = [&]() -> ComPtr<ID3D11Texture2D> {
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        ComPtr<IDXGIResource> resource;
        duplicator->ReleaseFrame();
        auto hr = duplicator->AcquireNextFrame(20, &frameInfo, &resource);
        DXGI_OUTDUPL_DESC desc;
        duplicator->GetDesc(&desc);
        if (FAILED(hr)) return nullptr;
        ComPtr<ID3D11Texture2D> texture2D;
        hr = resource.As(&texture2D);
        if (FAILED(hr)) return nullptr;
        return texture2D;
    };

    auto GetHighPrecisionTime = [&]() -> double {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        return elapsed.count();
    };

    bool running = true;
    const double targetFrameTime = 1.0 / 120.0; // fps
    double lastFrameTime = GetHighPrecisionTime();
    MSG msg = {};

    while (running) {

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT || msg.message == WM_CLOSE) {
                running = false;
                break;
            }
        }

        double currentTime = GetHighPrecisionTime();
        double deltaTime = currentTime - lastFrameTime;

        if (deltaTime >= targetFrameTime) {
            auto texture2d = captureFrame();
            d3dContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
            d3dContext->CopyResource(backBuffer.Get(), texture2d.Get());
            swapChain->Present(0, 0);
            
            lastFrameTime = currentTime;
        } else {
            Sleep(1);
        }
    }

    return 0;
}
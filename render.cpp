#include "render.h"
#include "utils.h"
#include "window.h"


DXGI_SWAP_CHAIN_DESC createSwapChainDesc(HWND hwnd, size_t captureWidth, size_t captureHeight)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = (UINT)captureWidth;
    sd.BufferDesc.Height = (UINT)captureHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    return sd;
}


void DefineTheViewPort(ComPtr<ID3D11DeviceContext> context, float width, float height)
{
    D3D11_VIEWPORT viewport;
    viewport.Width = width;  // width of the render target
    viewport.Height = height;  // height of the render target
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    context->RSSetViewports(1, &viewport);
}


Renderer::Renderer(long captureWidth, long captureHeight, DXGI_FORMAT)
    : m_captureWidth(captureWidth)
    , m_captureHeight(captureHeight)
{
    m_hwnd = createWindow();
    auto initialize = [&] () -> HRESULT {
        ChkRet(CreateDeviceAndSwapChain());
        ChkRet(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_backBuffer.GetAddressOf())));
        ChkRet(m_device->CreateRenderTargetView(m_backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));
        m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
        DefineTheViewPort(m_deviceContext, static_cast<float>(m_captureWidth), static_cast<float>(m_captureHeight));
        ChkRet(SetBackground());
        return S_OK;
    };
    if (FAILED(initialize())) {
        std::terminate();
    }
}


HRESULT Renderer::CreateDeviceAndSwapChain()
{
    const std::vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 
    };
    m_swapChainDesc = createSwapChainDesc(m_hwnd, m_captureWidth, m_captureHeight);
    D3D_FEATURE_LEVEL lvl;
    return D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels.data(), (UINT)featureLevels.size(), D3D11_SDK_VERSION,
        &m_swapChainDesc, &m_swapChain,
        &m_device, &lvl, &m_deviceContext);
}


HRESULT Renderer::SetBackground() {
    static float r = 0.0f;
    static float g = 0.2f;
    static float b = 0.4f;
    static float delta = 0.02f;
    const float clearColor[4] = {r, g, b, 1.0f};
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    ChkRet(m_swapChain->Present(1, 0));
    r += delta;
    if (r > 1.0f || r < 0.0f) {
        delta *= -1;
    }
    return S_OK;
}


HRESULT Renderer::StartRenderLoop(std::function<HRESULT(ComPtr<ID3D11Texture2D>& texture2D)> captureFn, const double fps)
{
    bool running = true;
    const double targetFrameTime = 1.0 / fps;
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
            ComPtr<ID3D11Texture2D> capturedTexture;
            auto hr = captureFn(capturedTexture);
            m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
            m_deviceContext->CopyResource(m_backBuffer.Get(), capturedTexture.Get());
            m_swapChain->Present(0, 0);
            
            lastFrameTime = currentTime;
        } else {
            Sleep(1);
        }
    }
    return S_OK;
}

// ComPtr<ID3D11Device> createDevice()
// {
//     const std::vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
//         D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
//         D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 
//     };

//     ComPtr<ID3D11Device> device;
//     D3D_FEATURE_LEVEL lvl;
//     ComPtr<ID3D11DeviceContext> context;

//     auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
//         featureLevels.data(), (UINT)featureLevels.size(), D3D11_SDK_VERSION,
//         &device, &lvl, &context);

//     return device;
// }
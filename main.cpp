#include "duplicator.h"
#include "render.h"
#include "window.h"
#include "utils.h"

#include <iostream>
#include <chrono>



int main()
{
    DXGI_OUTPUT_DESC1 desc1 = getMainDisplayDesc1();
    auto captureWidth = desc1.DesktopCoordinates.right - desc1.DesktopCoordinates.left;
    auto captureHeight = desc1.DesktopCoordinates.bottom - desc1.DesktopCoordinates.top;
    
    Renderer renderer(captureWidth, captureHeight, DXGI_FORMAT_B8G8R8A8_UNORM);
    ComPtr<IDXGIOutputDuplication> duplicator = GetDuplicatorFromDevice(renderer.GetD3DDevice());

    auto captureFrame = [&](ComPtr<ID3D11Texture2D>& texture2D) -> HRESULT {
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        ComPtr<IDXGIResource> resource;
        duplicator->ReleaseFrame();
        HRESULT hr = duplicator->AcquireNextFrame(20, &frameInfo, &resource);
        if (FAILED(hr) && hr != DXGI_ERROR_WAIT_TIMEOUT) ChkRet(hr);
        ChkRet(resource.As(&texture2D));
        return S_OK;
    };

    const double fps = 60;
    ChkRet(renderer.StartRenderLoop(captureFrame, fps));

    return 0;
}
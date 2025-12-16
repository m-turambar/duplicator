
#include "d3d11.h"
#include "d2d1_1.h"
#include "wrl.h"
#include "dxgi1_6.h"

#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <tuple>

using Microsoft::WRL::ComPtr;

struct ToneMapper
{
    static std::optional<ToneMapper> Create(DXGI_OUTPUT_DESC1& desc1, ComPtr<ID3D11Device>& d3dDevice);
    ToneMapper(DXGI_OUTPUT_DESC1& desc1, ComPtr<ID3D11Device>& d3dDevice);

    HRESULT Map(ComPtr<ID3D11Texture2D> input, ComPtr<ID3D11Texture2D>& output);
    HRESULT CreateEffects();

    DXGI_OUTPUT_DESC1 desc1;
    // DISPLAYCONFIG_SDR_WHITE_LEVEL sdrWhiteLevel;
    ULONG SDRWhiteLevel = 1000;

    ComPtr<ID3D11Device> d3dDevice;
    ComPtr<IDXGIDevice> dxgiDevice;
    ComPtr<ID2D1Device> d2dDevice;
    ComPtr<ID2D1Factory1> factory;
    ComPtr<ID2D1Effect> bgraToNv12Effect;
    ComPtr<ID2D1Effect> colorEffect;
};
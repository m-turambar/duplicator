#include "d3d11.h"
#include "wrl.h"
#include "dxgi1_2.h"

#pragma once

#include <vector>
#include <tuple>

using Microsoft::WRL::ComPtr;

DXGI_SWAP_CHAIN_DESC createSwapChainDesc(HWND hwnd, size_t captureWidth, size_t captureHeight);

ComPtr<ID3D11Device> createDevice();
std::tuple<ComPtr<ID3D11DeviceContext>, ComPtr<ID3D11Device>, ComPtr<IDXGISwapChain>>
    createDeviceAndSwapChain(HWND hwnd, size_t captureWidth, size_t captureHeight);

void defineTheViewPort(ComPtr<ID3D11DeviceContext> context, int width, int height);

ComPtr<IDXGIOutputDuplication> GetDuplicatorFromDevice(ComPtr<ID3D11Device> device);
std::pair<UINT, UINT> getMainDisplayDimensions();
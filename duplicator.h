#pragma once

#include "d3d11.h"
#include "d2d1_1.h"
#include "wrl.h"
#include "dxgi1_6.h"

#include <memory>
#include <optional>
#include <vector>
#include <tuple>

using Microsoft::WRL::ComPtr;



ComPtr<IDXGIOutputDuplication> GetDuplicatorFromDevice(ComPtr<ID3D11Device> device);
DXGI_OUTPUT_DESC1 getMainDisplayDesc1();
bool didDuplicationFail(HRESULT hr);

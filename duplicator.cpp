#include "duplicator.h"
#include "utils.h"

DXGI_OUTPUT_DESC1 getMainDisplayDesc1()
{
    ComPtr<IDXGIFactory1> dxgiFactory;
    ComPtr<IDXGIAdapter1> adapter;
    CreateDXGIFactory1(__uuidof(IDXGIFactory1), &dxgiFactory);
    dxgiFactory->EnumAdapters1(0, &adapter);
    ComPtr<IDXGIOutput> dxgiOutput;
    adapter->EnumOutputs(0, &dxgiOutput);
    ComPtr<IDXGIOutput6> output6;
    dxgiOutput.As(&output6);
    DXGI_OUTPUT_DESC1 desc1;
    output6->GetDesc1(&desc1);
    return desc1;
}

ComPtr<IDXGIOutputDuplication> GetDuplicatorFromDevice(ComPtr<ID3D11Device> device)
{
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    ChkRetVal(device.As(&dxgiDevice), nullptr);
    Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
    ChkRetVal(dxgiDevice->GetAdapter(&dxgiAdapter), nullptr);
    Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput;
    ChkRetVal(dxgiAdapter->EnumOutputs(0, &dxgiOutput), nullptr);
    Microsoft::WRL::ComPtr<IDXGIOutput1> dxgiOutput1;
    ChkRetVal(dxgiOutput.As(&dxgiOutput1), nullptr);
    Microsoft::WRL::ComPtr<IDXGIOutputDuplication> dxgiOutputDuplication;
    ChkRetVal(dxgiOutput1->DuplicateOutput(device.Get(), &dxgiOutputDuplication), nullptr);
    return dxgiOutputDuplication;
}

bool didDuplicationFail(HRESULT hr)
{
    if (hr == DXGI_ERROR_WAIT_TIMEOUT || hr == S_OK) {
        return false;
    }
    return true;
}

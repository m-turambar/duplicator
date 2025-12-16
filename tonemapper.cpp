std::optional<ToneMapper> ToneMapper::Create(DXGI_OUTPUT_DESC1& desc1, ComPtr<ID3D11Device>& d3dDevice)
{
    try {
        ToneMapper tm(desc1, d3dDevice);
        if FAILED(tm.CreateEffects()) { return std::nullopt; }
        return tm;
    } catch (...) {
        return std::nullopt;
    }
}

ToneMapper::ToneMapper(DXGI_OUTPUT_DESC1& desc1, ComPtr<ID3D11Device>& d3dDevice)
    : desc1(desc1)
    , d3dDevice(d3dDevice)
{
    if FAILED(d3dDevice.As(&dxgiDevice)) throw std::exception{};
    //whiteLevel();
}


HRESULT ToneMapper::Map(ComPtr<ID3D11Texture2D> input, ComPtr<ID3D11Texture2D>& output)
{
    return E_FAIL;
    return S_OK;
}

HRESULT ToneMapper::CreateEffects()
{
    auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory.ReleaseAndGetAddressOf());
    if FAILED(hr) return hr;

    hr = factory->CreateDevice(dxgiDevice.Get(), d2dDevice.ReleaseAndGetAddressOf());

    // factory->CreateEffect(CLSID_D2D1ColorMatrix, &colorEffect);

    return S_OK;
}

void whiteLevel()
{
    UINT32 numPathArrayElems; 
	UINT32 numModeInfoArrayElems;
	GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElems, &numModeInfoArrayElems);

	DISPLAYCONFIG_PATH_INFO *pathArray = new DISPLAYCONFIG_PATH_INFO[numPathArrayElems];
	DISPLAYCONFIG_MODE_INFO *modeInfoArray = new DISPLAYCONFIG_MODE_INFO[numModeInfoArrayElems];

	QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &numPathArrayElems, pathArray, &numModeInfoArrayElems, modeInfoArray, nullptr);

	// print white level for all displays
	for(size_t i = 0; i < numPathArrayElems; i++){
		DISPLAYCONFIG_SDR_WHITE_LEVEL request;
		request.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SDR_WHITE_LEVEL;
		request.header.size = sizeof(request);
		request.header.adapterId = pathArray->targetInfo.adapterId;
		request.header.id = pathArray->targetInfo.id;

		DisplayConfigGetDeviceInfo(reinterpret_cast<DISPLAYCONFIG_DEVICE_INFO_HEADER*>(&request));
		
		std::cout << "White level: " << request.SDRWhiteLevel << std::endl;
	}
}

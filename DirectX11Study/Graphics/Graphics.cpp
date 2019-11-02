#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height){
	mWindowWidth = width;
	mWindowHeight = height;
	mFpsTimer.Start();

	if (!InitializeDirectX(hwnd)) {
		return false;
	}

	if (!InitializeShaders()) {
		return false;
	}

	if (!InitializeScene()) {
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(mDevice.Get(), mDeviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::RenderFrame() {
	float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	gameObject.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (mFpsTimer.GetMilesecondsElapsed() > 1000.0) {
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		mFpsTimer.Restart();
	}

	mSpriteBatch->Begin();
	mSpriteFont->DrawString(mSpriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), XMFLOAT2(0, 0), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	mSpriteBatch->End();


	static int counter = 0;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	//ImGui::DragFloat3("Translation X/Y/Z", translationOffset, 0.1f, -5.0f, 5.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	mSwapChain->Present(0, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd) {
	try {
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		if (adapters.empty()) {
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scd = { 0 };

		scd.BufferDesc.Width = mWindowWidth;
		scd.BufferDesc.Height = mWindowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapters[0].pAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			NULL,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&scd,
			mSwapChain.GetAddressOf(),
			mDevice.GetAddressOf(),
			NULL,
			mDeviceContext.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed");

		hr = mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, mWindowWidth, mWindowHeight);
		textureDesc.MipLevels = 1;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = mDevice->CreateTexture2D(&textureDesc, NULL, mDepthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, mDepthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		// create depth stencil state
		CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state");

		// create & set the viewport
		CD3D11_VIEWPORT viewPort(0.0f, 0.0f, static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight));
		mDeviceContext->RSSetViewports(1, &viewPort);

		//rasterizer state
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		hr = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//rasterizer state cull front
		CD3D11_RASTERIZER_DESC rasterizerCullFrontDesc(D3D11_DEFAULT);
		rasterizerCullFrontDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		hr = mDevice->CreateRasterizerState(&rasterizerCullFrontDesc, mRasterizerStateCullFront.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		CD3D11_SAMPLER_DESC sampleDesc(D3D11_DEFAULT);
		sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = mDevice->CreateSamplerState(&sampleDesc, mSamplerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state");

		D3D11_BLEND_DESC blendDesc = { 0 };

		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };

		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbd;
		hr = mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

		mSpriteBatch = std::make_unique<SpriteBatch>(mDeviceContext.Get());
		mSpriteFont = std::make_unique<SpriteFont>(mDevice.Get(), L"Data/Fonts/consolas.spritefont");
	}
	catch (COMException& exception) {
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders() {
	std::wstring shaderFolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE) {
#ifdef _DEBUG
	#ifdef _WIN64
		shaderFolder = L"../x64/Debug/";
	#else
		shaderFolder = L"../Debug/";
	#endif
#else
	#ifdef _WIN64
		shaderFolder = L"../x64/Release/";
	#else
			shaderFolder = L"../Release/";
	#endif
#endif // _DEBUG
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!mVertexShader.Initialize(mDevice, shaderFolder + L"vertexShader.cso", layout, numElements)) {
		return false;
	}

	if (!mPixelShader.Initialize(mDevice, shaderFolder + L"pixelShader.cso")) {
		return false;
	}

	return true;
}

bool Graphics::InitializeScene() {
	try {
		HRESULT hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data\\Textures\\pinksquare.jpg", nullptr, mPinkTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, mGrassTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(mDevice.Get(), L"Data\\Textures\\seamless_pavement.jpg", nullptr, mPavementTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = cb_vs_vertexshader.Initialize(mDevice.Get(), mDeviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initailize constant buffer");

		hr = cb_ps_pixelshader.Initialize(mDevice.Get(), mDeviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initailize constant buffer");

		//initailize model(s)
		if (!gameObject.Initailize("Data/Objects/nanosuit/nanosuit.obj", mDevice.Get(), mDeviceContext.Get(), mGrassTexture.Get(), cb_vs_vertexshader)) {
			return false;
		}

		camera.SetPosition(0.0f, 0.0f, -2.0f);
		camera.SetProjectionValues(90.0f, static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight), 0.1f, 1000.0f);
	}
	catch (COMException& exception) {
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}
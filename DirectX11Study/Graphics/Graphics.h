#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "GameObject.h"
#include "Camera.h"
#include "..//Timer.h"
#include "ImGui//imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera camera;
	GameObject gameObject;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	int mWindowHeight;
	int mWindowWidth;

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	VertexShader mVertexShader;
	PixelShader mPixelShader;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerStateCullFront;

	Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mPinkTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mGrassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mPavementTexture;

	std::unique_ptr<SpriteBatch> mSpriteBatch;
	std::unique_ptr<SpriteFont> mSpriteFont;

	Timer mFpsTimer;
};
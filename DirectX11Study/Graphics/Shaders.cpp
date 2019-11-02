#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements) {
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to load shader: " + shaderPath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), NULL, mShader.GetAddressOf());
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to create vertex shader: " + shaderPath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numElements, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), mInputLayout.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create input layout.");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader() {
	return mShader.Get();
}

ID3D10Blob* VertexShader::GetBuffer() {
	return mShaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout() {
	return mInputLayout.Get();
}


bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath) {
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to load shader: " + shaderPath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(mShaderBuffer.Get()->GetBufferPointer(), mShaderBuffer.Get()->GetBufferSize(), NULL, mShader.GetAddressOf());
	if (FAILED(hr)) {
		std::wstring errorMsg = L"Failed to create pixel shader: " + shaderPath;
		ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader() {
	return mShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer() {
	return mShaderBuffer.Get();
}
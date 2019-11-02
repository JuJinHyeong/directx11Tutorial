#pragma once
#include "Mesh.h"

using namespace DirectX;

class Model {
private:
	std::vector<Mesh> mMeshes;
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* mCbVsVertexShader = nullptr;
	ID3D11ShaderResourceView* mTexture = nullptr;

public:
	bool Initailize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexShader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);

};
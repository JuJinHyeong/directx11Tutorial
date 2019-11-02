#include "Model.h"

bool Model::Initailize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexShader)
{
	mDevice = device;
	mDeviceContext = deviceContext;
	mTexture = texture;
	mCbVsVertexShader = &cbVsVertexShader;
	try {
		if (!LoadModel(filePath)) {

		}
	}
	catch (COMException& exception) {
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture) {
	mTexture = texture;
}

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{
	mCbVsVertexShader->data.mat = worldMatrix * viewProjectionMatrix;
	mCbVsVertexShader->data.mat = XMMatrixTranspose(mCbVsVertexShader->data.mat);
	mCbVsVertexShader->ApplyChanges();
	mDeviceContext->VSSetConstantBuffers(0, 1, mCbVsVertexShader->GetAddressOf());

	mDeviceContext->PSSetShaderResources(0, 1, &mTexture);
	
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].Draw();
	}
}

bool Model::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath,
											  aiProcess_Triangulate |
											  aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr) {
		return false;
	}

	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	//get vertex
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
			vertex.texCoord.y = static_cast<float>(mesh->mTextureCoords[0][i].y);
		}

		vertices.push_back(vertex);
	}

	//get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(mDevice, mDeviceContext, vertices, indices);
}
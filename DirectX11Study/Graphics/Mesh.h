#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>

class Mesh {
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
	Mesh(const Mesh& mesh);
	void Draw();
private:
	VertexBuffer<Vertex> mVertexBuffer;
	IndexBuffer mIndexBuffer;
	ID3D11DeviceContext* mDeviceContext;
};
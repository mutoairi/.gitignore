#pragma once
#include"makeMatrix.h"
#include"ModelCommon.h"
#include<Windows.h>
#include<string>
#include <iostream>
#include <fstream>
#include<vector>
#include<wrl.h>
#include<cstdint>
#include<d3d12.h>

struct MaterialData
{
	std::string texturFilePath;
	uint32_t textureIndex = 0;
};
struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};

class Model
{
public:
	void Initialize(ModelCommon* modelCommon);
	void Draw();

	//.mtlファイルの読み取り
	struct MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	//.objファイルの読み取り
	struct ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
private:
	void Vertex();
	void MaterialInitialize();
	ModelCommon* modelCommon_;

	//OBJファイルデータ
	ModelData modelData;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//	マテリアルにデータを書き込む
	Material* materialData = nullptr;
};


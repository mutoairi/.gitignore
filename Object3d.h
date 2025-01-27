#pragma once
#include"makeMatrix.h"
#include<Windows.h>
#include<string>
#include<vector>
#include<wrl.h>
#include<cstdint>
#include<d3d12.h>



struct MaterialData
{
	std::string texturFilePath;
	uint32_t textureIndex=0;
};
struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};

class Object3dCommon;
class Object3d
{
public:
	void Initialize(Object3dCommon*object3dCommon);
	void Update();
	void Draw();

	//.mtlファイルの読み取り
	struct MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み取り
	struct ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
private:
	void Vertex();
	void MaterialInitialize();
	void TransformInitialize();
	void EnableLightInitialize();
	Object3dCommon* object3dCommon=nullptr;
	//OBJファイルデータ
	ModelData modelData;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	//wvp用のリソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//	マテリアルにデータを書き込む
	Material* materialData = nullptr;
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	//平行光源
	DirectionalLight* directionalLightData = nullptr;
	Transform transform;
	Transform cameraTransform;


};


#pragma once
#include"makeMatrix.h"
#include<Windows.h>
#include<string>
#include<vector>
#include<wrl.h>
#include<cstdint>
#include<d3d12.h>
#include"Model.h"



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
	void SetModel(Model* model) {
		this->model = model;
	}
	
private:
	
	void TransformInitialize();
	void EnableLightInitialize();
	Object3dCommon* object3dCommon=nullptr;
	
	//wvp用のリソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;

	
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	//平行光源
	DirectionalLight* directionalLightData = nullptr;
	Transform transform;
	Transform cameraTransform;
	Model* model = nullptr;


};


#pragma once
#include"makeMatrix.h"
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<math.h>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include"Logger.h"
#include"StringUtility.h"


struct Material
{
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

class SpriteCommon;
class Sprite
{
public:
	void Initialize(SpriteCommon*spriteCommon);

	void Update();

	void Draw(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);

private:
	void VertexInitialize();
	void IndexInitialize();
	void MaterialInitialize();
	void TransformationInitialize();
	void TransformationUpdate(Transform transformationSprite);
	void UVTransform(Transform transformationSprite);

	SpriteCommon* spriteCommon = nullptr;
	//バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResourceSprite;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResourceSprite;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResourceSprite;

	//バッファリソース内のデータを指すポイント
	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//インデックスにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	//	マテリアルにデータを書き込む
	Material* materialDataSprite = nullptr;
	//トランスフォーメーションのデータを書き込む
	TransformationMatrix* transformationMatrixDataSprite = nullptr;
	

	//バッファリソースの使い道を補足するバッファービュー
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
};


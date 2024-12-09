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
#include"TextureManager.h"


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
	void Initialize(SpriteCommon*spriteCommon,std::string textureFilePath);

	void Update();

	void Draw();

	//getter
	const Vector2& GetPosition()const { return position; }
	float GetRotation()const { return rotation; }
	const Vector4& GetColor()const { return materialDataSprite->color; }
	const Vector2& GetSize()const { return size; }

	//setter
	void SetPosition(const Vector2& position) { this->position = position; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetColor(const Vector4& color) { materialDataSprite->color = color; }
	void SetSize(const Vector2& size) { this->size = size; }

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
	//テクスチャ番号
	uint32_t textureIndex = 0;

	Vector2 position = { 0.0f,0.0f };
	Vector2 size = { 640.0f,360.0f };
	float rotation = 0.0f;

	//バッファリソースの使い道を補足するバッファービュー
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
};

